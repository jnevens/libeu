/*
 * socket.h
 *
 *  Created on: Jun 20, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_BUS_SOCKET_H_
#define INCLUDE_BUS_SOCKET_H_

#include <stdbool.h>
#include <eu/types.h>

typedef enum
{
	SOCKET_TYPE_TCP,
	SOCKET_TYPE_UDP,
	SOCKET_TYPE_UNIX,
} socket_type_e;

socket_t *socket_create_unix(void);
socket_t *socket_create_tcp(void);
socket_t *socket_create_udp(void);

bool socket_bind_unix(socket_t *sock, const char *path);
bool socket_bind_tcp(socket_t *sock, uint16_t port);
bool socket_bind_udp(socket_t *sock, uint16_t port);

bool socket_connect_unix(socket_t *sock, const char *path);
bool socket_connect_tcp(socket_t *sock, uint16_t port);
bool socket_connect_udp(socket_t *sock, uint16_t port);

bool socket_listen(socket_t *sock, int n);
bool socket_is_copy(socket_t *sock);
int socket_get_fd(socket_t *sock);
socket_type_e socket_get_type(socket_t *sock);
bool socket_set_blocking(socket_t *sock, bool blocking);
socket_t *socket_accept(socket_t *sock);
size_t socket_write(socket_t *sock, uint8_t *data, size_t len);
ssize_t socket_read(socket_t *sock, uint8_t *data, size_t len);

void socket_set_userdata(socket_t *sock, void *userdata);
void *socket_get_userdata(socket_t *sock);

#endif /* INCLUDE_BUS_SOCKET_H_ */
