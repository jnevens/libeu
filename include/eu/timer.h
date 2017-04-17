/*
 * timer.h
 *
 *  Created on: Apr 16, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_EU_TIMER_H_
#define INCLUDE_EU_TIMER_H_

#include <stdbool.h>
#include "types.h"

#define eu_event_timer_continue	true
#define eu_event_timer_stop		false

eu_event_timer_t *eu_event_timer_create(uint32_t timeout_ms, bool (*callback)(void *arg), void *arg);
void eu_event_timer_set_userdata(eu_event_timer_t *timer, void *arg);
void *eu_event_timer_get_userdata(eu_event_timer_t *timer);
void eu_event_timer_destroy(eu_event_timer_t *timer);

long long time_ms(void);

#endif /* INCLUDE_EU_TIMER_H_ */
