/*
 * bus_message.c
 *
 *  Created on: Jul 10, 2016
 *      Author: jnevens
 */
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <eu/bus_message.h>



eu_bus_message_t *eu_bus_message_create(eu_bus_msg_type_t type, uint8_t *data, size_t datalen)
{
	eu_bus_message_t *msg = calloc(1, sizeof(eu_bus_message_t) + datalen);
	if(msg) {
		msg->trans_id = (unsigned)((void *)data - NULL) ^ (unsigned)((void *)msg - NULL);
		msg->type = type;
		msg->len = datalen;
		memcpy(msg->data, data, datalen);
	}
	return msg;
}

size_t eu_bus_message_size(eu_bus_message_t *msg)
{
	return sizeof(eu_bus_message_t) + msg->len;
}

void eu_bus_message_destroy(eu_bus_message_t *msg)
{
	if(msg) {
		free(msg->data);
		free(msg);
	}
}
