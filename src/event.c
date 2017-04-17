/*
 * event.c
 *
 *  Created on: Apr 16, 2016
 *      Author: jnevens
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>

#include "eu/list.h"
#include "eu/event.h"
#include "eu/log.h"

static bool keep_running;
list_t *events;

struct event_s
{
	int fd;
	void (*callback)(int fd, short int events, void *arg);
	void (*err_callback)(int fd, short int events, void *arg);
	void *arg;
	short int events;
	int nfdn;
	bool deleted;
};

bool event_loop_init(void)
{
	events = list_create();

	if (!events) {
		return false;
	}

	return true;
}

static void events_cleanup(bool all)
{
	list_node_t *node;
	list_for_each(node, events) {
		event_t *event = list_node_data(node);
		if(event->deleted || all) {
			list_remove_node(events, node);
			free(event);
			break;
		}
	}
}

void event_loop_cleanup(void) {
	events_cleanup(true);
	list_destroy(events);
	events = NULL;
}

event_t *event_add(int fd, short int qevents,
		void (*callback)(int fd, short int revents, void *arg),
		void (*err_callback)(int fd, short int revents, void *arg),
		void *arg)
{
	event_t *event = calloc(1, sizeof(event_t));
	if (event) {
		event->fd = fd;
		event->callback = callback;
		event->err_callback = err_callback;
		event->arg = arg;
		event->deleted = false;
		event->events = qevents;
		event->nfdn = -1;
		list_append(events, event);
	}

	return event;
}

void event_destroy(event_t *event)
{
	event->deleted = true;
}

static int fd_handlers_count(void)
{
	return (nfds_t)list_count(events);
}

static void fd_handlers_build_fd_set(struct pollfd fdset[])
{
	list_node_t *node;
	nfds_t n = 0;

	list_for_each(node, events) {
		event_t *event = list_node_data(node);
		event->nfdn = n;
		fdset[n].fd = event->fd;
		fdset[n].events = event->events;
		n++;
	}
}

void event_loop(void)
{
	keep_running = true;

	while (keep_running) {
		struct pollfd *fdset = NULL;
		int rv;

		/* get max fd */
		nfds_t nfds = fd_handlers_count();

		/* create poll struct */
		fdset = calloc(nfds, sizeof(struct pollfd));

		/* build fdset */
		fd_handlers_build_fd_set(fdset);

		/* so select */
		rv = poll(fdset, nfds, 1000);

		if (rv < 0) { /* select error! */
			perror("poll");
		} else if (rv > 0) { /* something happened! */
			list_node_t *node;
			list_for_each(node, events) {
				event_t *event = list_node_data(node);

				if (event->nfdn >= 0) {
					if (fdset[event->nfdn].revents & event->events) {
						if(!event->deleted && event->callback) {
							event->callback(event->fd, fdset[event->nfdn].revents, event->arg);
						}
					}
				}
				event->nfdn = -1;
			}
		}
		events_cleanup(false);
		free(fdset);
	}
}

void event_loop_stop(void) {
	keep_running = false;
}
