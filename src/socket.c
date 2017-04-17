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

struct socket_s {
	socket_type_e	type;
	bool	copy;
	int fd;
	void *userdata;
};

socket_t *socket_create_unix(void)
{
	socket_t *sock = calloc(1, sizeof(socket));
	sock->type = SOCKET_TYPE_UNIX;

	sock->fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock->fd < 0) {
		fprintf(stderr, "Cannot create socket!\n");
		return NULL;
	}

	log_debug("unix socket created: fd=%d", sock->fd);
	return sock;
}

static socket_t *socket_duplicate(socket_t *sock)
{
	socket_t *new = calloc(1, sizeof(socket_t));
	if(!new) {
		return NULL;
	}

	memcpy(new, sock, sizeof(socket_t));
	new->copy = true;
	return new;
}

socket_t *socket_create_tcp(void)
{
	socket_t *socket = calloc(1, sizeof(socket));
	socket->type = SOCKET_TYPE_TCP;
	return socket;
}

socket_t *socket_create_udp(void)
{
	socket_t *socket = calloc(1, sizeof(socket));
	socket->type = SOCKET_TYPE_UDP;
	return socket;
}

bool socket_bind_unix(socket_t *sock, const char *path)
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

bool socket_bind_tcp(socket_t *sock, uint16_t port)
{
	return false;
}

bool socket_bind_udp(socket_t *sock, uint16_t port)
{
	return false;
}

bool socket_connect_unix(socket_t *sock, const char *path)
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

bool socket_connect_tcp(socket_t *sock, uint16_t port)
{
	return false;
}

bool socket_connect_udp(socket_t *sock, uint16_t port)
{
	return false;
}

bool socket_listen(socket_t *sock, int n)
{
	return (listen(sock->fd, n) == 0) ? true : false;
}

bool socket_is_copy(socket_t *sock)
{
	return sock->copy;
}

int socket_get_fd(socket_t *sock)
{
	return sock->fd;
}

socket_type_e socket_get_type(socket_t *socket)
{
	return socket->type;
}

bool socket_set_blocking(socket_t *sock, bool blocking)
{
	int flags = fcntl(sock->fd, F_GETFL, 0);

	if(blocking) {
		flags &= ~O_NONBLOCK;
	} else {
		flags |= O_NONBLOCK;
	}

	return (fcntl(sock->fd, F_SETFL, flags) == 0) ? true : false;
}

socket_t *socket_accept(socket_t *sock)
{
	int fd = accept(sock->fd, NULL, NULL);

	if(fd < 0) {
		return NULL;
	}

	socket_t *new = socket_duplicate(sock);
	new->fd = fd;
	return new;
}

size_t socket_write(socket_t *sock, uint8_t *data, size_t len)
{
	// TODO loop until everything is written
	int rv = write(sock->fd, data, len);

	return (rv == len) ? true : false;
}

ssize_t socket_read(socket_t *sock, uint8_t *data, size_t len)
{
	// TODO loop until everything is read
	return read(sock->fd, data, len);
}

void socket_set_userdata(socket_t *sock, void *userdata)
{
	sock->userdata = userdata;
}

void *socket_get_userdata(socket_t *sock)
{
	return sock->userdata;
}
