/*
 * socket.h
 *
 *  Created on: Jun 20, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_EU_SOCKET_H_
#define INCLUDE_EU_SOCKET_H_

#include <stdbool.h>
#include <eu/types.h>

typedef enum
{
	EU_SOCKET_TYPE_TCP,
	EU_SOCKET_TYPE_UDP,
	EU_SOCKET_TYPE_UNIX,
} eu_socket_type_e;

eu_socket_t *eu_socket_create_unix(void);
eu_socket_t *eu_socket_create_tcp(void);
eu_socket_t *eu_socket_create_udp(void);

bool eu_socket_bind_unix(eu_socket_t *sock, const char *path);
bool eu_socket_bind_tcp(eu_socket_t *sock, uint16_t port);
bool eu_socket_bind_udp(eu_socket_t *sock, uint16_t port);

bool eu_socket_connect_unix(eu_socket_t *sock, const char *path);
bool eu_socket_connect_tcp(eu_socket_t *sock, uint16_t port);
bool eu_socket_connect_udp(eu_socket_t *sock, uint16_t port);

bool eu_socket_listen(eu_socket_t *sock, int n);
bool eu_socket_is_copy(eu_socket_t *sock);
int eu_socket_get_fd(eu_socket_t *sock);
eu_socket_type_e eu_socket_get_type(eu_socket_t *sock);
bool eu_socket_set_blocking(eu_socket_t *sock, bool blocking);
eu_socket_t *eu_socket_accept(eu_socket_t *sock);
size_t eu_socket_write(eu_socket_t *sock, uint8_t *data, size_t len);
ssize_t eu_socket_read(eu_socket_t *sock, uint8_t *data, size_t len);

void eu_socket_set_userdata(eu_socket_t *sock, void *userdata);
void *eu_socket_get_userdata(eu_socket_t *sock);

#endif /* INCLUDE_EU_SOCKET_H_ */
