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

eu_socket_t *socket_create_unix(void);
eu_socket_t *socket_create_tcp(void);
eu_socket_t *socket_create_udp(void);

bool socket_bind_unix(eu_socket_t *sock, const char *path);
bool socket_bind_tcp(eu_socket_t *sock, uint16_t port);
bool socket_bind_udp(eu_socket_t *sock, uint16_t port);

bool socket_connect_unix(eu_socket_t *sock, const char *path);
bool socket_connect_tcp(eu_socket_t *sock, uint16_t port);
bool socket_connect_udp(eu_socket_t *sock, uint16_t port);

bool socket_listen(eu_socket_t *sock, int n);
bool socket_is_copy(eu_socket_t *sock);
int socket_get_fd(eu_socket_t *sock);
socket_type_e socket_get_type(eu_socket_t *sock);
bool socket_set_blocking(eu_socket_t *sock, bool blocking);
eu_socket_t *socket_accept(eu_socket_t *sock);
size_t socket_write(eu_socket_t *sock, uint8_t *data, size_t len);
ssize_t socket_read(eu_socket_t *sock, uint8_t *data, size_t len);

void socket_set_userdata(eu_socket_t *sock, void *userdata);
void *socket_get_userdata(eu_socket_t *sock);

#endif /* INCLUDE_BUS_SOCKET_H_ */
