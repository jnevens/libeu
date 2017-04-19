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


eu_bus_conn_t *eu_bus_connect(const char *name);
eu_object_t *eu_bus_register_path(eu_bus_conn_t * conn, const char *path);


#endif /* INCLUDE_EU_CLIENT_H_ */
