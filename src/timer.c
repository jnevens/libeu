/*
 * timer.c
 *
 *  Created on: Apr 16, 2016
 *      Author: jnevens
 */
#include <stdio.h>
#include <stdint.h>
#include <sys/timerfd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>

#include "eu/log.h"
#include "eu/event.h"
#include "eu/timer.h"

struct eu_event_timer_s
{
	uint32_t timeout_ms;
	eu_event_t *event;
	int timerfd;
	bool (*callback)(void *arg);
	void *arg;
};

static void event_timer_callback(int fd, short int revents, void *arg)
{
	eu_event_timer_t *timer = arg;
	bool keep_running = false;
	long long unsigned buf = 0;

	ssize_t rv = read(timer->timerfd, &buf, sizeof(buf));
	if (rv != 8) {
		log_err("Problem reading timerfd! %d %s", rv, strerror(errno));
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
		log_err("Failed allocating timer!");
		exit(-1);
	}
	timer->timerfd = timerfd_create(CLOCK_MONOTONIC, 0);
	log_debug("timerfd: %d", timer->timerfd);

	timspec.it_interval.tv_sec = (timeout_ms > 999) ? timeout_ms / 1000 : 0;
	timspec.it_interval.tv_nsec = (timeout_ms % 1000) * 1000000;
	timspec.it_value.tv_sec = (timeout_ms > 999) ? timeout_ms / 1000 : 0;
	timspec.it_value.tv_nsec = (timeout_ms % 1000) * 1000000;

	int res = timerfd_settime(timer->timerfd, 0, &timspec, NULL);
	if (res < 0) {
		log_err("timerfd_settime");
		perror("timerfd_settime");
	}

	timer->event = eu_event_add(timer->timerfd, POLLIN, event_timer_callback, NULL, timer);
	timer->callback = callback;
	timer->arg = arg;

	log_info("timer created: %d.%03ds", timspec.it_interval.tv_sec, timspec.it_interval.tv_nsec / 1000000);

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
	close(timer->timerfd);
	free(timer);
}

long long eu_time_ms(void)
{
	long long time = 0;
	struct timespec spec;

	clock_gettime(CLOCK_REALTIME, &spec);

	time = (long long)spec.tv_sec * 1000 + (long long)spec.tv_nsec / 1000000;

	return time;
}
