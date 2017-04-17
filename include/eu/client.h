/*
 * client.h
 *
 *  Created on: Jul 10, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_EU_CLIENT_H_
#define INCLUDE_EU_CLIENT_H_

#include <stdbool.h>
#include <eu/types.h>

bool eu_bus_connect(void);
eu_object_t *eu_bus_register_path(const char *path);


#endif /* INCLUDE_EU_CLIENT_H_ */
