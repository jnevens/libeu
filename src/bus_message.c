/*
 * bus_message.c
 *
 *  Created on: Jul 10, 2016
 *      Author: jnevens
 */
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <bus/bus_message.h>



bus_message_t *bus_message_create(bus_msg_type_t type, uint8_t *data, size_t datalen)
{
	bus_message_t *msg = calloc(1, sizeof(bus_message_t) + datalen);
	if(msg) {
		msg->trans_id = (unsigned)((void *)data - NULL) ^ (unsigned)((void *)msg - NULL);
		msg->type = type;
		msg->len = datalen;
		memcpy(msg->data, data, datalen);
	}
	return msg;
}

size_t bus_message_size(bus_message_t *msg)
{
	return sizeof(bus_message_t) + msg->len;
}

void bus_message_destroy(bus_message_t *msg)
{
	if(msg) {
		free(msg->data);
		free(msg);
	}
}
