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

#define eu_event_timer_continue	true
#define eu_event_timer_stop		false

/* event loop */
bool eu_event_loop_init(void);
void eu_event_loop(void);
void eu_event_loop_stop(void);
void eu_event_loop_cleanup(void);

/* fd events */
eu_event_t *eu_event_add(int fd, short int events,
		void (*callback)(int fd, short int revents, void *arg),
		void (*err_callback)(int fd, short int revents, void *arg),
		void *arg);

void *eu_event_get_userdata(eu_event_t *event);
void eu_event_set_userdata(eu_event_t *event, void *userdata);

void eu_event_destroy(eu_event_t *event);

/* event timer */
eu_event_timer_t *eu_event_timer_create(uint32_t timeout_ms, bool (*callback)(void *arg), void *arg);
void eu_event_timer_set_userdata(eu_event_timer_t *timer, void *arg);
void *eu_event_timer_get_userdata(eu_event_timer_t *timer);
void eu_event_timer_destroy(eu_event_timer_t *timer);

/* help functions */
long long time_ms(void);


#endif /* INCLUDE_EVENT_H_ */
