/*
 * eu_client.c
 *
 *  Created on: Jul 10, 2016
 *      Author: jnevens
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <eu/client.h>
#include <eu/socket.h>
#include <eu/log.h>
#include <eu/event.h>
#include <eu/object.h>

#include <eu/bus_message.h>

struct eu_bus_conn_s {
	eu_socket_t *sock;
	eu_object_t *root;
	char *name;
};

static bool eu_bus_request_object(eu_bus_conn_t *conn, eu_bus_message_t *msg)
{
	eu_bus_message_request_object_response_t *bus_msg_resp = NULL;
	size_t bus_msg_resp_size = 0;

	// get bus message request
	eu_bus_message_request_object_t *bus_msg_req = eu_bus_message_data(msg);

	// find object
	eu_log_debug("Look for path: %s", bus_msg_req->path);
	eu_object_t *obj = eu_object_get_child_path(conn->root, bus_msg_req->path);
	if (obj) {
		json_object *jobj = eu_object_serialize(obj, eu_object_serialize_attrs_none);
		const char *json = json_object_to_json_string(jobj);
		bus_msg_resp_size = sizeof(eu_bus_message_request_object_response_t) + strlen(json) + 1;
		bus_msg_resp = calloc(1, bus_msg_resp_size);
		bus_msg_resp->status = EU_BUS_SUCCESS;
		strcpy(bus_msg_resp->object, json);
		json_object_put(jobj);
	} else {
		// create error response
		bus_msg_resp_size = sizeof(eu_bus_message_request_object_response_t);
		bus_msg_resp = calloc(1, bus_msg_resp_size);
		bus_msg_resp->status = EU_BUS_NO_SUCH_OBJECT;
	}

	eu_bus_message_t *reply = eu_bus_message_create(EU_BUS_REQUEST_OBJECT_RESPONSE,
			bus_msg_resp, bus_msg_resp_size);
	reply->trans_id = msg->trans_id;
	eu_socket_write(conn->sock, reply, eu_bus_message_size(reply));
	free(bus_msg_resp);
	return true;
}

static void handle_message(eu_bus_conn_t *conn, eu_bus_message_t *msg)
{
	switch (msg->type) {
	case EU_BUS_REGISTER_CLIENT_RESPONSE:
	{
		int response = EU_BUS_REGISTER_CLIENT_INVALID;
		memcpy(&response, msg->data, sizeof(eu_bus_register_client_response_t));
		if (response == EU_BUS_REGISTER_CLIENT_SUCCESS) {
			eu_log_info("Successfully registered path!");
		} else if (response == EU_BUS_REGISTER_CLIENT_FAILED_ALREADY_IN_USE) {
			eu_log_err("Failed to register path: Already in use!");
		} else {
			eu_log_err("Failed to register path!");
		}
		break;
	}
	case EU_BUS_REQUEST_OBJECT:
	{
		eu_bus_request_object(conn, msg);
		break;
	}
	default:
		eu_log_err("Unsupported action: %d", msg->type);
		break;
	}
}

static eu_bus_message_t *bus_conn_read_message(eu_bus_conn_t *conn){
	size_t msg_header_size = sizeof(eu_bus_message_t);
	eu_bus_message_t *msg = calloc(1, msg_header_size);
	eu_socket_t *sock = conn->sock;

	/* try to read header */
	int rv = eu_socket_read(sock, (uint8_t *) msg, msg_header_size);
	if (rv != msg_header_size) {
		eu_log_err("Failed reading header!");
		free(msg);
		return NULL;
	}

	/* read data */
	msg = realloc(msg, msg_header_size + msg->data_len);
	rv = eu_socket_read(sock, &((uint8_t *) msg)[msg_header_size], msg->data_len);
	if (rv != msg->data_len) {
		eu_log_err("Failed reading message data");
	}

	return msg;
}

static void bus_connection_callback(int fd, short int revents, void *arg) {
	eu_bus_message_t *msg = NULL;
	eu_bus_conn_t *conn = (eu_bus_conn_t *) arg;


	eu_log_info("connection callback!");

	// read message;
	msg = bus_conn_read_message(conn);
	if(!msg) {
		eu_log_info("Connection closed!");
	} else {
		handle_message(conn, msg);
		eu_bus_message_destroy(msg);
	}
}

eu_bus_conn_t *eu_bus_connect(const char *name) {
	eu_socket_t *sock = eu_socket_create_unix();

	// try to connect to eu bus
	if (!eu_socket_connect_unix(sock, "/tmp/eubd.sock")) {
		eu_log_err("Failed connecting socket!");
		return NULL;
	}

	eu_bus_conn_t *conn = calloc(1, sizeof(eu_bus_conn_t));
	conn->sock = sock;
	conn->name = strdup(name);

	eu_event_add(eu_socket_get_fd(sock), POLLIN, bus_connection_callback, NULL, conn);

	return conn;
}

eu_object_t *eu_bus_register_client(eu_bus_conn_t *conn, const char *path)
{
	/* create & send request */
	eu_bus_message_t *msg = eu_bus_message_create(EU_BUS_REGISTER_CLIENT,
			(uint8_t *) path, strlen(path) + 1);
	eu_socket_write(conn->sock, msg, eu_bus_message_size(msg));

	/* read response */
	eu_bus_message_t *resp = bus_conn_read_message(conn);
	int response = EU_BUS_REGISTER_CLIENT_INVALID;
	memcpy(&response, resp->data, sizeof(eu_bus_register_client_response_t));
	if (response == EU_BUS_REGISTER_CLIENT_SUCCESS) {
		eu_log_info("Successfully registered path!");
		conn->root = eu_object_create_path(NULL, path);
	} else if (response == EU_BUS_REGISTER_CLIENT_FAILED_ALREADY_IN_USE) {
		eu_log_err("Failed to register path: Already in use!");
	} else {
		eu_log_err("Failed to register path!");
	}

	eu_bus_message_destroy(msg);
	eu_bus_message_destroy(resp);
	return conn->root;
}

eu_object_t *eu_bus_list_object(eu_bus_conn_t *conn, const char *path, eu_bus_status_t *status)
{
	eu_object_t *obj = NULL;

	/* create & send request */
	size_t path_len = strlen(path);
	size_t req_len = sizeof(eu_bus_message_request_object_t) + path_len + 1;
	eu_bus_message_request_object_t *req = calloc(1, req_len);
	req->options = EU_BUS_REQUEST_OBJECT_FLAG_NONE;
	sprintf(req->path, "%s", path);

	eu_bus_message_t *msg = eu_bus_message_create(EU_BUS_REQUEST_OBJECT, req, req_len);
	eu_socket_write(conn->sock, msg, eu_bus_message_size(msg));

	/* read response */
	eu_bus_message_t *resp = bus_conn_read_message(conn);
	eu_bus_message_request_object_response_t *bus_msg =  eu_bus_message_data(resp);
	*status = bus_msg->status;
	if (bus_msg->status == EU_BUS_SUCCESS) {
		const char *json = bus_msg->object;
		json_object *jobj = json_tokener_parse(json);
		obj = eu_object_deserialize(NULL, jobj);
	}

	eu_bus_message_destroy(msg);
	eu_bus_message_destroy(resp);
	return obj;
}
