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
eu_list_t *events;

struct eu_event_s
{
	int fd;
	void (*callback)(int fd, short int events, void *arg);
	void (*err_callback)(int fd, short int events, void *arg);
	void *arg;
	short int events;
	int nfdn;
	bool deleted;
};

bool eu_event_loop_init(void)
{
	events = eu_list_create();

	if (!events) {
		return false;
	}

	return true;
}

static void events_cleanup(bool all)
{
	eu_list_node_t *node;
	eu_list_for_each(node, events) {
		eu_event_t *event = eu_list_node_data(node);
		if(event->deleted || all) {
			eu_list_remove_node(events, node);
			free(event);
			break;
		}
	}
}

void eu_event_loop_cleanup(void) {
	events_cleanup(true);
	eu_list_destroy(events);
	events = NULL;
}

eu_event_t *eu_event_add(int fd, short int qevents,
		void (*callback)(int fd, short int revents, void *arg),
		void (*err_callback)(int fd, short int revents, void *arg),
		void *arg)
{
	eu_event_t *event = calloc(1, sizeof(eu_event_t));
	if (event) {
		event->fd = fd;
		event->callback = callback;
		event->err_callback = err_callback;
		event->arg = arg;
		event->deleted = false;
		event->events = qevents;
		event->nfdn = -1;
		eu_list_append(events, event);
	}

	return event;
}

void eu_event_destroy(eu_event_t *event)
{
	event->deleted = true;
}

static int fd_handlers_count(void)
{
	return (nfds_t)eu_list_count(events);
}

static void fd_handlers_build_fd_set(struct pollfd fdset[])
{
	eu_list_node_t *node;
	nfds_t n = 0;

	eu_list_for_each(node, events) {
		eu_event_t *event = eu_list_node_data(node);
		event->nfdn = n;
		fdset[n].fd = event->fd;
		fdset[n].events = event->events;
		n++;
	}
}

void eu_event_loop(void)
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
			eu_list_node_t *node;
			eu_list_for_each(node, events) {
				eu_event_t *event = eu_list_node_data(node);

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

void eu_event_loop_stop(void) {
	keep_running = false;
}
