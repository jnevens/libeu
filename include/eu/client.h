/*
 * client.h
 *
 *  Created on: Jul 10, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_BUS_CLIENT_H_
#define INCLUDE_BUS_CLIENT_H_

#include <stdbool.h>
#include <eu/types.h>

bool bus_connect(void);
eu_object_t *bus_register_path(const char *path);


#endif /* INCLUDE_BUS_CLIENT_H_ */
