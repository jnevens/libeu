/*
 * daemonize.h
 *
 *  Created on: Jun 9, 2013
 *      Author: jnevens
 */

#ifndef DAEMON_H_
#define DAEMON_H_

#include <stdbool.h>

bool eu_daemonize(const char *pidfile);

#endif /* DAEMON_H_ */
