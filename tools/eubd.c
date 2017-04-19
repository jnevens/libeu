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

#include <eu/event.h>
#include <eu/daemon.h>
#include <eu/log.h>
#include <eu/list.h>
#include <eu/socket.h>
#include <eu/bus_message.h>

eu_object_t *root = NULL;

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
	char *path;
	eu_socket_t *client_sock;
} registered_path_t;
static eu_list_t *registerd_paths = NULL;

static eu_bus_register_path_response_t eubd_register_client_path(
		eu_socket_t *client, const char *path) {
	// lookup list to find duplicate
	eu_list_node_t *node = NULL;
	eu_list_for_each(node, registerd_paths)
	{
		registered_path_t *lpath = eu_list_node_data(node);
		if (strcmp(lpath->path, path) == 0) {
			return EU_BUS_REGISTER_PATH_FAILED_ALREADY_IN_USE;
		}
	}

	registered_path_t *regpath = calloc(1, sizeof(registered_path_t));
	if (regpath) {
		regpath->path = strdup(path);
		regpath->client_sock = client;
		eu_list_append(registerd_paths, regpath);
		return EU_BUS_REGISTER_PATH_SUCCESS;
	} else {
		return EU_BUS_REGISTER_PATH_FAILED;
	}
}

static void eubd_unregister_client(eu_socket_t *client)
{
	// remote from registered paths
	eu_event_t *event = eu_socket_get_userdata(client);
	eu_event_destroy(event);
	// remove event!
	eu_list_node_t *node = eu_list_first(registerd_paths);
	while(node)
	{
		eu_list_node_t *nextnode = eu_list_node_next(node);
		registered_path_t *lpath = eu_list_node_data(node);
		if(lpath->client_sock == client) {
			log_info("unregister path: %s", lpath->path);
			free(lpath);
			eu_list_remove_node(registerd_paths, node);
		}
		node = nextnode;
	}
}

static void message_handler(eu_socket_t *client, eu_bus_message_t *msg)
{
	switch(msg->type) {
		case EU_BUS_REGISTER_PATH :
		{
			eu_bus_register_path_response_t status = eubd_register_client_path(client, (char *)msg->data);
			log_info("Register path: %s %d", (char *)msg->data, (int)status);
			eu_bus_message_t *newmsg = eu_bus_message_create(EU_BUS_REGISTER_PATH_RESPONSE,
					&status, sizeof(eu_bus_register_path_response_t));
			newmsg->trans_id = msg->trans_id;
			eu_socket_write(client, newmsg, eu_bus_message_size(newmsg));
			break;
		}
		default:
			log_err("Unsupported action: %d", msg->type);
			break;
	}
}

void eud_connection_callback(int fd, short int revents, void *arg)
{
	size_t msg_header_size = sizeof(eu_bus_message_t);
	eu_bus_message_t *msg = calloc(1, msg_header_size);
	eu_socket_t *client = arg;
	eu_event_t *event = eu_socket_get_userdata(client);

	// read header
	int rv = eu_socket_read(client, (uint8_t *) msg, msg_header_size);
	if (rv != msg_header_size) {
		log_err("Failed reading header!");
	} else {
		// read data
		msg = realloc(msg, msg_header_size + msg->len);
		rv = eu_socket_read(client, &((uint8_t *) msg)[msg_header_size],
				msg->len);
		if (rv != msg->len) {
			log_err("Failed reading message data");
		}
	}

	if(rv == 0) {
		log_info("Connection closed!");
		eubd_unregister_client(client);
		eu_event_destroy(event);
	} else {
		message_handler(client, msg);
		eu_bus_message_destroy(msg);
	}
}

void eud_server_callback(int fd, short int revents, void *arg)
{
	eu_socket_t *server = arg;
	log_info("server callback!");
	eu_socket_t *new = eu_socket_accept(server);
	eu_event_t *event = eu_event_add(eu_socket_get_fd(new), POLLIN, eud_connection_callback, NULL, new);
	eu_socket_set_userdata(new, event);
}

int main(int argc, char *argv[])
{
	eu_log_init("eud");
	eu_event_loop_init();

	log_info("hello!");

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	if (arguments.verbose) {
		eu_log_set_print_level(EU_LOG_DEBUG);
	}

	if (arguments.daemonize) {
		eu_daemonize("/var/run/eubd.pid");
	}

	eu_socket_t *server = eu_socket_create_unix();
	if(!server) {
		log_err("Failed creating server socket!");
		exit(-1);
	}
	unlink("/tmp/eud.sock");
	if(!eu_socket_bind_unix(server, "/tmp/eubd.sock")) {
		exit(-1);
	}

	registerd_paths = eu_list_create();

	eu_socket_listen(server, 10);
	log_debug("fd = %d", eu_socket_get_fd(server));
	eu_event_add(eu_socket_get_fd(server), POLLIN, eud_server_callback, NULL, server);

	eu_event_loop();
	eu_event_loop_cleanup();
	return 0;
}

