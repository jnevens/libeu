/*
 * timer.h
 *
 *  Created on: Apr 16, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_BUS_TIMER_H_
#define INCLUDE_BUS_TIMER_H_

#include <stdbool.h>
#include "types.h"

#define event_timer_continue	true
#define event_timer_stop		false

event_timer_t *event_timer_create(uint32_t timeout_ms, bool (*callback)(void *arg), void *arg);
void event_timer_set_userdata(event_timer_t *timer, void *arg);
void *event_timer_get_userdata(event_timer_t *timer);
void event_timer_destroy(event_timer_t *timer);

long long time_ms(void);

#endif /* INCLUDE_BUS_TIMER_H_ */
