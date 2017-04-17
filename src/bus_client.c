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

#include <eu/bus_message.h>

eu_socket_t *conn = NULL;

static void bus_connection_callback(int fd, short int revents, void *arg)
{
	log_info("connection callback!");
}

bool eu_bus_connect(void)
{
	conn = eu_socket_create_unix();
	if(!eu_socket_connect_unix(conn, "/var/run/busd.sock")) {
		log_err("Failed connecting socket!");
		return false;
	}
	log_debug("fd = %d", eu_socket_get_fd(conn));
	//socket_write(client, "test", 5);
	eu_event_add(eu_socket_get_fd(conn), POLLIN, bus_connection_callback, NULL, NULL);

	return true;
}

eu_object_t *eu_bus_register_path(const char *path)
{
	eu_bus_message_t *msg = eu_bus_message_create(REGISTER_PATH, (uint8_t *)path, strlen(path) + 1);
	eu_socket_write(conn, msg, eu_bus_message_size(msg));
	return NULL;
}
