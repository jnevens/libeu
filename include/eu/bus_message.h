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
	REGISTER_PATH,
	REGISTER_PATH_RESPONSE,
	REQUEST_OBJECT,
	REQUEST_OBJECT_RESPONSE,
	REQUEST_OBJECT_PARAMETER,
	REQUEST_OBJECT_PARAMETER_RESPONSE,
} eu_bus_msg_type_t;

typedef struct eu_bus_message_s
{
	eu_bus_msg_type_t type;
	size_t len;
	uint32_t trans_id;
	uint8_t data[];
} eu_bus_message_t;


eu_bus_message_t *eu_bus_message_create(eu_bus_msg_type_t type, uint8_t *data, size_t datalen);
void eu_bus_message_destroy(eu_bus_message_t *msg);

size_t eu_bus_message_size(eu_bus_message_t *msg);



#endif /* SRC_EU_MESSAGE_H_ */
