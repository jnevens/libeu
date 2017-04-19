/*
 * eu_client.c
 *
 *  Created on: Jul 10, 2016
 *      Author: jnevens
 */
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
	char *name;
};

static void handle_message(eu_bus_conn_t *conn, eu_bus_message_t *msg) {
	switch (msg->type) {
	case EU_BUS_REGISTER_PATH_RESPONSE: {
		int response = EU_BUS_REGISTER_PATH_INVALID;
		memcpy(&response, msg->data, sizeof(eu_bus_register_path_response_t));
		if (response == EU_BUS_REGISTER_PATH_SUCCESS) {
			log_info("Successfully registered path!");
		} else if (response == EU_BUS_REGISTER_PATH_FAILED_ALREADY_IN_USE) {
			log_err("Failed to register path: Already in use!");
		} else {
			log_err("Failed to register path!");
		}
		break;
	}
	default:
		log_err("Unsupported action: %d", msg->type);
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
		log_err("Failed reading header!");
		free(msg);
		return NULL;
	}

	/* read data */
	msg = realloc(msg, msg_header_size + msg->len);
	rv = eu_socket_read(sock, &((uint8_t *) msg)[msg_header_size], msg->len);
	if (rv != msg->len) {
		log_err("Failed reading message data");
	}

	return msg;
}

static void bus_connection_callback(int fd, short int revents, void *arg) {
	eu_bus_message_t *msg = NULL;
	eu_bus_conn_t *conn = (eu_bus_conn_t *) arg;


	log_info("connection callback!");

	// read message;
	msg = bus_conn_read_message(conn);
	if(!msg) {
		log_info("Connection closed!");
	} else {
		handle_message(conn, msg);
		eu_bus_message_destroy(msg);
	}
}

eu_bus_conn_t *eu_bus_connect(const char *name) {
	eu_socket_t *sock = eu_socket_create_unix();

	// try to connect to eu bus
	if (!eu_socket_connect_unix(sock, "/tmp/eubd.sock")) {
		log_err("Failed connecting socket!");
		return NULL;
	}

	eu_bus_conn_t *conn = calloc(1, sizeof(eu_bus_conn_t));
	conn->sock = sock;
	conn->name = strdup(name);

	eu_event_add(eu_socket_get_fd(sock), POLLIN, bus_connection_callback, NULL, conn);

	return conn;
}

eu_object_t *eu_bus_register_path(eu_bus_conn_t *conn, const char *path)
{
	eu_object_t *obj = NULL;
	/* create & send request */
	eu_bus_message_t *msg = eu_bus_message_create(EU_BUS_REGISTER_PATH,
			(uint8_t *) path, strlen(path) + 1);
	eu_socket_write(conn->sock, msg, eu_bus_message_size(msg));

	/* read response */
	eu_bus_message_t *resp = bus_conn_read_message(conn);
	int response = EU_BUS_REGISTER_PATH_INVALID;
	memcpy(&response, resp->data, sizeof(eu_bus_register_path_response_t));
	if (response == EU_BUS_REGISTER_PATH_SUCCESS) {
		log_info("Successfully registered path!");
		obj = eu_object_create_path(NULL, path);
	} else if (response == EU_BUS_REGISTER_PATH_FAILED_ALREADY_IN_USE) {
		log_err("Failed to register path: Already in use!");
	} else {
		log_err("Failed to register path!");
	}

	eu_bus_message_destroy(msg);
	eu_bus_message_destroy(resp);
	return obj;
}
