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



eu_bus_message_t *eu_bus_message_create(eu_bus_msg_type_t type, void *data, size_t datalen)
{
	eu_bus_message_t *msg = calloc(1, sizeof(eu_bus_message_t) + datalen);
	if(msg) {
		msg->trans_id = (unsigned)((void *)data - NULL) ^ (unsigned)((void *)msg - NULL);
		msg->type = type;
		msg->data_len = datalen;
		memcpy(msg->data, data, datalen);
	}
	return msg;
}

eu_bus_message_t *eu_bus_message_duplicate(eu_bus_message_t *orig)
{
	eu_bus_message_t *new = NULL;

	new = eu_bus_message_create(orig->type, orig->data, orig->data_len);
	new->trans_id = orig->trans_id;

	return new;
}

size_t eu_bus_message_size(eu_bus_message_t *msg)
{
	return sizeof(eu_bus_message_t) + msg->data_len;
}

void *eu_bus_message_data(eu_bus_message_t *msg)
{
	return msg->data;
}

size_t eu_bus_message_data_len(eu_bus_message_t *msg)
{
	return msg->data_len;
}

uint32_t eu_bus_message_id(eu_bus_message_t *msg)
{
	return msg->trans_id;
}

eu_bus_msg_type_t eu_bus_message_type(eu_bus_message_t *msg)
{
	return msg->type;
}

void eu_bus_message_destroy(eu_bus_message_t *msg)
{
	if(msg) {
		free(msg);
	}
}

