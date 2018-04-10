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
#include <eu/bus_message.h>

typedef void (*eu_bus_conn_disconnect_cb_t)(eu_bus_conn_t *conn, void *arg);

eu_bus_conn_t *eu_bus_connect(const char *name);
void eu_bus_disconnect(eu_bus_conn_t *conn);
void eu_bus_set_disconnect_handler(eu_bus_conn_t *conn, eu_bus_conn_disconnect_cb_t cb, void *arg);
eu_object_t *eu_bus_register_client(eu_bus_conn_t *conn, const char *client);
eu_object_t *eu_bus_list_object(eu_bus_conn_t *conn, const char *path, eu_bus_status_t *status);


#endif /* INCLUDE_EU_CLIENT_H_ */
