/*
 * socket.c
 *
 *  Created on: Jun 20, 2016
 *      Author: jnevens
 */
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

#include <eu/log.h>
#include <eu/socket.h>

struct eu_socket_s {
	eu_socket_type_e	type;
	bool	copy;
	int fd;
	void *userdata;
};

eu_socket_t *eu_socket_create_unix(void)
{
	eu_socket_t *sock = calloc(1, sizeof(socket));
	sock->type = EU_SOCKET_TYPE_UNIX;

	sock->fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock->fd < 0) {
		fprintf(stderr, "Cannot create socket!\n");
		return NULL;
	}

	log_debug("unix socket created: fd=%d", sock->fd);
	return sock;
}

static eu_socket_t *socket_duplicate(eu_socket_t *sock)
{
	eu_socket_t *new = calloc(1, sizeof(eu_socket_t));
	if(!new) {
		return NULL;
	}

	memcpy(new, sock, sizeof(eu_socket_t));
	new->copy = true;
	return new;
}

eu_socket_t *eu_socket_create_tcp(void)
{
	eu_socket_t *socket = calloc(1, sizeof(socket));
	socket->type = EU_SOCKET_TYPE_TCP;
	return socket;
}

eu_socket_t *eu_socket_create_udp(void)
{
	eu_socket_t *socket = calloc(1, sizeof(socket));
	socket->type = EU_SOCKET_TYPE_UDP;
	return socket;
}

bool eu_socket_bind_unix(eu_socket_t *sock, const char *path)
{
	unlink(path);
	struct sockaddr_un server_un;

	server_un.sun_family = AF_UNIX;
	strcpy(server_un.sun_path, path);
	if (bind(sock->fd, (struct sockaddr *) &server_un, sizeof(struct sockaddr_un))) {
		log_err("Cannot bind socket! error=%s", strerror(errno));
		free(sock);
		return false;
	}

	return true;
}

bool eu_socket_bind_tcp(eu_socket_t *sock, uint16_t port)
{
	return false;
}

bool eu_socket_bind_udp(eu_socket_t *sock, uint16_t port)
{
	return false;
}

bool eu_socket_connect_unix(eu_socket_t *sock, const char *path)
{
	struct sockaddr_un server_un;
	server_un.sun_family = AF_UNIX;
	strcpy(server_un.sun_path, path);

	if (connect(sock->fd, (struct sockaddr *) &server_un, sizeof(struct sockaddr_un)) < 0) {
		close(sock->fd);
		log_err("Cannot connect stream socket! %s\n", strerror(errno));
		return false;
	}

	return true;
}

bool eu_socket_connect_tcp(eu_socket_t *sock, uint16_t port)
{
	return false;
}

bool eu_socket_connect_udp(eu_socket_t *sock, uint16_t port)
{
	return false;
}

bool eu_socket_listen(eu_socket_t *sock, int n)
{
	return (listen(sock->fd, n) == 0) ? true : false;
}

bool eu_socket_is_copy(eu_socket_t *sock)
{
	return sock->copy;
}

int eu_socket_get_fd(eu_socket_t *sock)
{
	return sock->fd;
}

eu_socket_type_e eu_socket_get_type(eu_socket_t *socket)
{
	return socket->type;
}

bool eu_socket_set_blocking(eu_socket_t *sock, bool blocking)
{
	int flags = fcntl(sock->fd, F_GETFL, 0);

	if(blocking) {
		flags &= ~O_NONBLOCK;
	} else {
		flags |= O_NONBLOCK;
	}

	return (fcntl(sock->fd, F_SETFL, flags) == 0) ? true : false;
}

eu_socket_t *eu_socket_accept(eu_socket_t *sock)
{
	int fd = accept(sock->fd, NULL, NULL);

	if(fd < 0) {
		return NULL;
	}

	eu_socket_t *new = socket_duplicate(sock);
	new->fd = fd;
	return new;
}

size_t eu_socket_write(eu_socket_t *sock, void *data, size_t len)
{
	// TODO loop until everything is written
	int rv = write(sock->fd, data, len);

	return (rv == len) ? true : false;
}

ssize_t eu_socket_read(eu_socket_t *sock, void *data, size_t len)
{
	// TODO loop until everything is read
	return read(sock->fd, data, len);
}

void eu_socket_set_userdata(eu_socket_t *sock, void *userdata)
{
	sock->userdata = userdata;
}

void *eu_socket_get_userdata(eu_socket_t *sock)
{
	return sock->userdata;
}
