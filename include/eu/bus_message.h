/*
 * bus_message.h
 *
 *  Created on: Jul 10, 2016
 *      Author: jnevens
 */

#ifndef SRC_EU_MESSAGE_H_
#define SRC_EU_MESSAGE_H_

#include <stdbool.h>

typedef enum {
	EU_BUS_REGISTER_PATH,
	EU_BUS_REGISTER_PATH_RESPONSE,
	EU_BUS_REQUEST_OBJECT,
	EU_BUS_REQUEST_OBJECT_RESPONSE,
	EU_BUS_REQUEST_OBJECT_PARAMETER,
	EU_BUS_REQUEST_OBJECT_PARAMETER_RESPONSE,
} eu_bus_msg_type_t;

typedef enum {
	EU_BUS_REGISTER_PATH_INVALID = 0,
	EU_BUS_REGISTER_PATH_SUCCESS,
	EU_BUS_REGISTER_PATH_FAILED_ALREADY_IN_USE,
	EU_BUS_REGISTER_PATH_FAILED,
} eu_bus_register_path_response_t;

typedef struct eu_bus_message_s
{
	eu_bus_msg_type_t type;
	size_t len;
	uint32_t trans_id;
	uint8_t data[];
} eu_bus_message_t;


eu_bus_message_t *eu_bus_message_create(eu_bus_msg_type_t type, void *data, size_t datalen);
void eu_bus_message_destroy(eu_bus_message_t *msg);

size_t eu_bus_message_size(eu_bus_message_t *msg);



#endif /* SRC_EU_MESSAGE_H_ */
