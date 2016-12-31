/*
 * bus_client.c
 *
 *  Created on: Jul 10, 2016
 *      Author: jnevens
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <bus/client.h>
#include <bus/socket.h>
#include <bus/log.h>
#include <bus/event.h>

#include <bus/bus_message.h>

socket_t *conn = NULL;

static void bus_connection_callback(int fd, short int revents, void *arg)
{
	log_info("connection callback!");
}

bool bus_connect(void)
{
	conn = socket_create_unix();
	if(!socket_connect_unix(conn, "/var/run/busd.sock")) {
		log_err("Failed connecting socket!");
		return false;
	}
	log_debug("fd = %d", socket_get_fd(conn));
	//socket_write(client, "test", 5);
	event_add(socket_get_fd(conn), POLLIN, bus_connection_callback, NULL, NULL);

	return true;
}

object_t *bus_register_path(const char *path)
{
	bus_message_t *msg = bus_message_create(REGISTER_PATH, (uint8_t *)path, strlen(path) + 1);
	socket_write(conn, msg, bus_message_size(msg));
	return NULL;
}
