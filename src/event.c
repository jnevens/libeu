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

#include "bus/list.h"
#include "bus/event.h"
#include "bus/log.h"

static bool keep_running;
list_t *events;

struct event_s
{
	int fd;
	void (*callback)(int fd, void *arg);
	void (*err_callback)(int fd, void *arg);
	void *arg;
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

event_t *event_add(int fd,
		void (*callback)(int fd, void *arg),
		void (*err_callback)(int fd, void *arg),
		void *arg)
{
	event_t *event = calloc(1, sizeof(event_t));
	if (event) {
		event->fd = fd;
		event->callback = callback;
		event->err_callback = err_callback;
		event->arg = arg;
		event->deleted = false;
		list_append(events, event);
	}

	log_debug("add event: fd=%d", event->fd);

	return event;
}

void event_destroy(event_t *event)
{
	event->deleted = true;
}



static void fd_handlers_build_fd_set(fd_set *rfds)
{
	list_node_t *node;
	FD_ZERO(rfds);

	list_for_each(node, events) {
		event_t *event = list_node_data(node);
		FD_SET(event->fd, rfds);
	}
}

static int fd_handlers_get_max_fd(fd_set *rfds)
{
	list_node_t *node;
	int maxfd = 0;

	list_for_each(node, events) {
		event_t *event = list_node_data(node);
		if(event->fd > maxfd)
			maxfd = event->fd;
	}
	return maxfd;
}

void event_loop(void)
{
	keep_running = true;

	while (keep_running) {
		int rv;
		fd_set rfds;

		/* build fd set */
		fd_handlers_build_fd_set(&rfds);

		/* get max fd */
		int max_fd = fd_handlers_get_max_fd(&rfds);

		/* so select */
		rv = select(max_fd + 1, &rfds, NULL, NULL, NULL);

		if (rv < 0) { /* select error! */
			perror("select");
		} else if (rv > 0) { /* something happened! */
			list_node_t *node;
			list_for_each(node, events) {
				event_t *event = list_node_data(node);
				if (FD_ISSET(event->fd, &rfds)) {
					if(event->callback) {
						event->callback(event->fd, event->arg);
					}
				}
			}
		}
		events_cleanup(false);
	}
}

void event_loop_stop(void) {
	keep_running = false;
}
