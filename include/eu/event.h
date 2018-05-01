/*
 * event.h
 *
 *  Created on: Apr 16, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_EVENT_H_
#define INCLUDE_EVENT_H_

#include <stdbool.h>
#include <stdint.h>
#include <poll.h>
#include "types.h"

bool eu_event_loop_init(void);
void eu_event_loop(void);
void eu_event_loop_stop(void);
void eu_event_loop_cleanup(void);

eu_event_t *eu_event_add(int fd, short int events,
		void (*callback)(int fd, short int revents, void *arg),
		void (*err_callback)(int fd, short int revents, void *arg),
		void *arg);

void *eu_event_get_userdata(eu_event_t *event);
void eu_event_set_userdata(eu_event_t *event, void *userdata);

void eu_event_destroy(eu_event_t *event);

#endif /* INCLUDE_EVENT_H_ */
