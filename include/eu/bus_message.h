/*
 * bus_message.h
 *
 *  Created on: Jul 10, 2016
 *      Author: jnevens
 */

#ifndef SRC_EU_MESSAGE_H_
#define SRC_EU_MESSAGE_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum {
	EU_BUS_SUCCESS = 0,
	EU_BUS_NO_SUCH_OBJECT,
} eu_bus_status_t;

typedef enum {
	EU_BUS_REGISTER_CLIENT,
	EU_BUS_REGISTER_CLIENT_RESPONSE,
	EU_BUS_REQUEST_OBJECT,
	EU_BUS_REQUEST_OBJECT_RESPONSE,
	EU_BUS_REQUEST_OBJECT_PARAMETER,
	EU_BUS_REQUEST_OBJECT_PARAMETER_RESPONSE,
} eu_bus_msg_type_t;

typedef enum {
	EU_BUS_REGISTER_CLIENT_INVALID = 0,
	EU_BUS_REGISTER_CLIENT_SUCCESS,
	EU_BUS_REGISTER_CLIENT_FAILED_ALREADY_IN_USE,
	EU_BUS_REGISTER_CLIENT_FAILED,
} eu_bus_register_client_response_t;

typedef struct eu_bus_message_s
{
	eu_bus_msg_type_t type;
	size_t data_len;
	uint32_t trans_id;
	uint8_t data[];
} eu_bus_message_t;

typedef enum {
	EU_BUS_REQUEST_OBJECT_FLAG_NONE = 0,
	EU_BUS_REQUEST_OBJECT_FLAG_RECURSIVE = 1,
} eu_bus_request_object_options_t;

typedef struct eu_bus_message_request_object_s
{
	eu_bus_request_object_options_t options;
	char path[];
} eu_bus_message_request_object_t;

typedef struct eu_bus_message_request_object_response_s
{
	eu_bus_status_t status;
	char object[];
} eu_bus_message_request_object_response_t;


eu_bus_message_t *eu_bus_message_create(eu_bus_msg_type_t type, void *data, size_t datalen);
eu_bus_message_t *eu_bus_message_duplicate(eu_bus_message_t *orig);
void eu_bus_message_destroy(eu_bus_message_t *msg);

size_t eu_bus_message_size(eu_bus_message_t *msg);
void *eu_bus_message_data(eu_bus_message_t *msg);
size_t eu_bus_message_data_len(eu_bus_message_t *msg);
uint32_t eu_bus_message_id(eu_bus_message_t *msg);
eu_bus_msg_type_t eu_bus_message_type(eu_bus_message_t *msg);


#endif /* SRC_EU_MESSAGE_H_ */
