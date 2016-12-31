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

bool event_loop_init(void);
void event_loop(void);
void event_loop_stop(void);
void event_loop_cleanup(void);

event_t *event_add(int fd, short int events,
		void (*callback)(int fd, short int revents, void *arg),
		void (*err_callback)(int fd, short int revents, void *arg),
		void *arg);

void event_destroy(event_t *event);

#endif /* INCLUDE_EVENT_H_ */
