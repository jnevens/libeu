/*
 * eud.c
 *
 *  Created on: Jun 20, 2016
 *      Author: jnevens
 */
#include <stdio.h>
#include <argp.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <error.h>

#include <eu/event.h>
#include <eu/daemon.h>
#include <eu/log.h>
#include <eu/list.h>
#include <eu/socket.h>
#include <eu/object.h>
#include <eu/bus_message.h>

eu_object_t *root_object = NULL;

/* Used by main to communicate with parse_opt. */
struct arguments
{
	bool daemonize;
	bool verbose;
};

/* Default arguments */
struct arguments arguments = {
	.daemonize = false,
	.verbose = false,
};

/* Program documentation. */
static char doc[] = "eud dispatch daemon";

/* A description of the arguments we accept. */
static char args_doc[] = "";

/* The options we understand. */
static struct argp_option options[] = {
	{ "deamonize", 'd', 0, 0, "Daemonize" },
	{ "verbose", 'v', 0, 0, "Verbose output" },
	{ 0 }
};

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	/* Get the input argument from argp_parse, which we
	 know is a pointer to our arguments structure. */
	struct arguments *arguments = state->input;

	switch (key)
	{
		case 'd':
			arguments->daemonize = true;
			break;
		case 'v':
			arguments->verbose = true;
			break;
		case ARGP_KEY_ARG:
			break;
		case ARGP_KEY_END:
			//argp_usage(state);
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };


typedef struct registered_path_s {
	eu_socket_t *socket;
} registered_path_t;

typedef struct request_object_pending_s {
	uint32_t id;
	eu_socket_t *socket;
} request_object_pending_t;
eu_list_t *requests_pending = NULL;

static eu_bus_register_client_response_t eubd_register_client( eu_socket_t *client, const char *client_name) {
	// lookup list to find duplicate
	eu_object_t *obj_client = eu_object_get_child(root_object, client_name);
	if (obj_client) {
		return EU_BUS_REGISTER_CLIENT_FAILED_ALREADY_IN_USE;
	}

	registered_path_t *regpath = calloc(1, sizeof(registered_path_t));
	if (regpath) {
		obj_client = eu_object_create(root_object, client_name, eu_object_attr_none);
		regpath->socket = client;
		eu_object_set_userdata(obj_client, regpath);
		return EU_BUS_REGISTER_CLIENT_SUCCESS;
	} else {
		return EU_BUS_REGISTER_CLIENT_FAILED;
	}
}

static void eubd_unregister_client(eu_socket_t *client)
{
	// remote from registered paths
	eu_log_debug("Unregister client: fd = %d", eu_socket_get_fd(client));
	eu_event_t *event = eu_socket_get_userdata(client);
	eu_event_destroy(event);

	eu_object_t *child = NULL;
	registered_path_t *lpath = NULL;
	eu_object_for_each_child(child, root_object) {
		lpath = eu_object_get_userdata(child);
		if(lpath && (lpath->socket == client)) {
			break;
		}
	}

	if (child == NULL) {
		eu_log_info("No object found for client! (fd = %d)", eu_socket_get_fd(client));
		return;
	}

	// Remove pending requests
	eu_list_node_t *node = eu_list_first(requests_pending);
	while(node) {
		eu_list_node_t *node_next = eu_list_node_next(node);
		request_object_pending_t *pen_req = eu_list_node_data(node);
		if (pen_req->socket == client) {
			eu_list_remove_node_with_data(requests_pending, node, free);
		}
		node = node_next;
	}

	// Remomve object
	free(lpath);
	eu_object_destroy_child(root_object, eu_object_name(child));
}

static bool eubd_request_object_is_local(const char *path)
{
	if (strcmp(path, "") == 0) {
		return true;
	} else if (strcmp(path, "System") == 0) {
		return true;
	}

	return false;
}

static void eubd_message_handle_register_client(eu_socket_t *client, eu_bus_message_t *msg)
{
	eu_bus_register_client_response_t status = eubd_register_client(client, (char *)msg->data);
	eu_log_info("Register path: %s %d", (char *)msg->data, (int)status);
	eu_bus_message_t *newmsg = eu_bus_message_create(EU_BUS_REGISTER_CLIENT_RESPONSE,
			&status, sizeof(eu_bus_register_client_response_t));
	newmsg->trans_id = msg->trans_id;
	eu_socket_write(client, newmsg, eu_bus_message_size(newmsg));
}

static void eubd_message_handle_request_object_local(eu_socket_t *client, eu_bus_message_t *msg)
{
	json_object *jobj = eu_object_serialize(root_object, eu_object_serialize_attrs_name_only);
	const char *json = json_object_to_json_string(jobj);
	size_t bus_msg_size = sizeof(eu_bus_message_request_object_response_t) + strlen(json) + 1;
	eu_bus_message_request_object_response_t *bus_msg = calloc(1, bus_msg_size);
	strcpy(bus_msg->object, json);
	eu_bus_message_t *reply = eu_bus_message_create(EU_BUS_REQUEST_OBJECT_RESPONSE, bus_msg, bus_msg_size);
	reply->trans_id = msg->trans_id;
	eu_socket_write(client, reply, eu_bus_message_size(reply));
	json_object_put(jobj);
	free(bus_msg);
}

static registered_path_t *eubd_lookup_client(const char *path)
{
	char *part = NULL;
	char *path_dup = strdup(path);

	eu_object_t *object = root_object;

	eu_log_debug("lookup client! %s", path);

	part = strtok(path_dup, ".");

	do {
		eu_log_debug("look for client... %s", part);
		object = eu_object_get_child(object, part);

		registered_path_t *reg = eu_object_get_userdata(object);
		if (reg) {
			eu_log_info("path found!");
			return reg;
		}
	} while((part = strtok(NULL, ".")) != NULL);

	return NULL;
}

static void eubd_message_handle_request_object_forward(eu_socket_t *client, eu_bus_message_t *msg)
{
	eu_bus_message_request_object_t *req = (void *)msg->data;

	eu_log_info("object forward: %s, options: %x", req->path, req->options);

	eu_bus_message_t *new = eu_bus_message_duplicate(msg);

	registered_path_t *reg_path = eubd_lookup_client(req->path);
	if (reg_path) {
		eu_socket_write(reg_path->socket, new, eu_bus_message_size(new));

		request_object_pending_t *req = calloc(1, sizeof(request_object_pending_t));
		req->id = new->trans_id;
		req->socket = client;
		eu_list_append(requests_pending, req);
	} else {
		// TODO: respond path not found!
	}
}

static void eubd_message_handle_request_object(eu_socket_t *client, eu_bus_message_t *msg)
{
	eu_bus_message_request_object_t *req = (void *)msg->data;
	eu_log_info("Request object: %s, options: %x", req->path, req->options);

	if(eubd_request_object_is_local(req->path)) {
		eubd_message_handle_request_object_local(client, msg);
	} else {
		eubd_message_handle_request_object_forward(client, msg);
	}
}

static void eubd_message_handle_request_object_response(eu_socket_t *client, eu_bus_message_t *msg)
{
	request_object_pending_t *pen_obj_req =  NULL;
	eu_log_info("Object request response received!");

	eu_list_node_t *node = NULL;
	eu_list_for_each(node, requests_pending) {
		pen_obj_req = eu_list_node_data(node);
		if (pen_obj_req->id == eu_bus_message_id(msg)) {
			eu_log_debug("Pending request found!");
			break;
		}
	}

	if (node) {
		pen_obj_req = eu_list_node_data(node);
		eu_bus_message_t *new = eu_bus_message_duplicate(msg);
		eu_socket_write(pen_obj_req->socket, new, eu_bus_message_size(new));
		eu_log_debug("Response sended!! %s", (char *)eu_bus_message_data(msg));
		eu_list_remove_node_with_data(requests_pending, node, free);
	}
}

static void eubd_message_handler(eu_socket_t *client, eu_bus_message_t *msg)
{
	switch(msg->type) {
		case EU_BUS_REGISTER_CLIENT :
			eubd_message_handle_register_client(client, msg);
			break;
		case EU_BUS_REQUEST_OBJECT :
			eubd_message_handle_request_object(client, msg);
			break;
		case EU_BUS_REQUEST_OBJECT_RESPONSE :
			eubd_message_handle_request_object_response(client, msg);
			break;
		default:
			eu_log_err("Unsupported action: %d", msg->type);
			break;
	}
}

static void eubd_connection_callback(int fd, short int revents, void *arg)
{
	size_t msg_header_size = sizeof(eu_bus_message_t);
	eu_bus_message_t *msg = calloc(1, msg_header_size);
	eu_socket_t *client = arg;
	eu_event_t *event = eu_socket_get_userdata(client);

	// read header
	int rv = eu_socket_read(client, (uint8_t *) msg, msg_header_size);
	if (rv != msg_header_size) {
		eu_log_err("Failed reading header!");
	} else {
		// read data
		msg = realloc(msg, msg_header_size + msg->data_len);
		rv = eu_socket_read(client, &((uint8_t *) msg)[msg_header_size],
				msg->data_len);
		if (rv != msg->data_len) {
			eu_log_err("Failed reading message data");
		}
	}

	if(rv == 0) {
		eu_log_info("Connection closed!");
		eubd_unregister_client(client);
	} else {
		eubd_message_handler(client, msg);
		eu_bus_message_destroy(msg);
	}
}

static void eubd_server_callback(int fd, short int revents, void *arg)
{
	eu_socket_t *server = arg;
	eu_log_info("server callback!");
	eu_socket_t *new = eu_socket_accept(server);
	eu_event_t *event = eu_event_add(eu_socket_get_fd(new), POLLIN, eubd_connection_callback, NULL, new);
	eu_socket_set_userdata(new, event);
}

static void eubd_initialize_data_model(void)
{
	root_object = eu_object_create(NULL, "root", eu_object_attr_none);
	eu_object_t *system_object = eu_object_create(root_object, "System", eu_object_attr_none);
	eu_object_create(system_object, "Clients", eu_object_attr_template);
}

int main(int argc, char *argv[])
{
	eu_log_init("eud");
	eu_event_loop_init();

	eu_log_info("hello!");

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	if (arguments.verbose) {
		eu_log_set_print_level(EU_LOG_DEBUG);
	}

	if (arguments.daemonize) {
		eu_daemonize("/var/run/eubd.pid");
	}

	requests_pending = eu_list_create();
	eubd_initialize_data_model();

	eu_socket_t *server = eu_socket_create_unix();
	if(!server) {
		eu_log_err("Failed creating server socket!");
		exit(-1);
	}
	unlink("/tmp/eud.sock");
	if(!eu_socket_bind_unix(server, "/tmp/eubd.sock")) {
		exit(-1);
	}

	eu_socket_listen(server, 10);
	eu_log_debug("fd = %d", eu_socket_get_fd(server));
	eu_event_add(eu_socket_get_fd(server), POLLIN, eubd_server_callback, NULL, server);

	eu_event_loop();
	eu_event_loop_cleanup();
	return 0;
}

