/*
 * bus_message.h
 *
 *  Created on: Jul 10, 2016
 *      Author: jnevens
 */

#ifndef SRC_BUS_MESSAGE_H_
#define SRC_BUS_MESSAGE_H_

#include <stdbool.h>

typedef enum {
	REGISTER_PATH,
	REGISTER_PATH_RESPONSE,
	REQUEST_OBJECT,
	REQUEST_OBJECT_RESPONSE,
	REQUEST_OBJECT_PARAMETER,
	REQUEST_OBJECT_PARAMETER_RESPONSE,
} bus_msg_type_t;

typedef struct bus_message_s
{
	bus_msg_type_t type;
	size_t len;
	uint32_t trans_id;
	uint8_t data[];
} bus_message_t;


bus_message_t *bus_message_create(bus_msg_type_t type, uint8_t *data, size_t datalen);
void bus_message_destroy(bus_message_t *msg);

size_t bus_message_size(bus_message_t *msg);



#endif /* SRC_BUS_MESSAGE_H_ */
