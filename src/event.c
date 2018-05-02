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
#include <stdint.h>
#include <sys/timerfd.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>

#include "eu/log.h"
#include "eu/list.h"
#include "eu/event.h"

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
	bool is_timer;
};

struct eu_event_timer_s
{
	uint32_t timeout_ms;
	eu_event_t *event;
	int timerfd;
	bool (*callback)(void *arg);
	void *arg;
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
	eu_list_node_t *node = NULL;
	eu_list_node_t *node_next = NULL;

	for(node = eu_list_first(events); node; node = node_next) {
		node_next = eu_list_node_next(node);
		eu_event_t *event = eu_list_node_data(node);
		if(event->deleted || all) {
			eu_list_remove_node(events, node);
			if (event->is_timer) {
				eu_event_timer_t *timer = eu_event_get_userdata(event);
				eu_event_timer_destroy(timer);
			}
			free(event);
			if (!all) break;
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

void *eu_event_get_userdata(eu_event_t *event)
{
	return event->arg;
}

void eu_event_set_userdata(eu_event_t *event, void *userdata)
{
	event->arg = userdata;
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


static void event_timer_callback(int fd, short int revents, void *arg)
{
	eu_event_timer_t *timer = arg;
	bool keep_running = false;
	long long unsigned buf = 0;

	ssize_t rv = read(timer->timerfd, &buf, sizeof(buf));
	if (rv != 8) {
		eu_log_err("Problem reading timerfd! %d %s", rv, strerror(errno));
	}

	if (timer->callback) {
		keep_running = timer->callback(timer->arg);
	}

	if(!keep_running) {
		eu_event_timer_destroy(timer);
	}
}

eu_event_timer_t *eu_event_timer_create(uint32_t timeout_ms, bool (*callback)(void *arg), void *arg)
{
	struct itimerspec timspec;
	bzero(&timspec, sizeof(timspec));

	eu_event_timer_t *timer = calloc(1, sizeof(eu_event_timer_t));
	if (!timer) {
		eu_log_err("Failed allocating timer!");
		exit(-1);
	}
	timer->timerfd = timerfd_create(CLOCK_MONOTONIC, 0);

	timspec.it_interval.tv_sec = (timeout_ms > 999) ? timeout_ms / 1000 : 0;
	timspec.it_interval.tv_nsec = (timeout_ms % 1000) * 1000000;
	timspec.it_value.tv_sec = (timeout_ms > 999) ? timeout_ms / 1000 : 0;
	timspec.it_value.tv_nsec = (timeout_ms % 1000) * 1000000;

	int res = timerfd_settime(timer->timerfd, 0, &timspec, NULL);
	if (res < 0) {
		eu_log_err("timerfd_settime");
		perror("timerfd_settime");
	}

	timer->event = eu_event_add(timer->timerfd, POLLIN, event_timer_callback, NULL, timer);
	timer->event->is_timer = true;
	timer->callback = callback;
	timer->arg = arg;

	return timer;
}

void eu_event_timer_set_userdata(eu_event_timer_t *timer, void *arg)
{
	timer->arg = arg;
}

void *eu_event_timer_get_userdata(eu_event_timer_t *timer)
{
	return timer->arg;
}

void eu_event_timer_destroy(eu_event_timer_t *timer)
{
	eu_event_destroy(timer->event);
}

long long eu_time_ms(void)
{
	long long time = 0;
	struct timespec spec;

	clock_gettime(CLOCK_REALTIME, &spec);

	time = (long long)spec.tv_sec * 1000 + (long long)spec.tv_nsec / 1000000;

	return time;
}
