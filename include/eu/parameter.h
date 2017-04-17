/*
 * parameter.h
 *
 *  Created on: Apr 10, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_BUS_PARAMETER_H_
#define INCLUDE_BUS_PARAMETER_H_

#include "eu/types.h"

typedef enum
{
	PARAMETER_TYPE_INVALID = 0, /* invalid! */
	PARAMETER_TYPE_BOOL,
	PARAMETER_TYPE_INT8,
	PARAMETER_TYPE_UINT8,
	PARAMETER_TYPE_INT16,
	PARAMETER_TYPE_UINT16,
	PARAMETER_TYPE_INT32,
	PARAMETER_TYPE_UINT32,
	PARAMETER_TYPE_INT64,
	PARAMETER_TYPE_UINT64,
	PARAMETER_TYPE_FLOAT,
	PARAMETER_TYPE_DOUBLE,
	PARAMETER_TYPE_STRING,
} parameter_type_t;

// create
parameter_t *parameter_create(object_t *obj, const char *name, parameter_type_t type);

// destroy
void parameter_destroy(parameter_t *param);

// info
const char *parameter_name(parameter_t *param);

// value
bool parameter_set_value(parameter_t *param, variant_t *variant);
variant_t *parameter_value(parameter_t *param);
variant_t *parameter_da_value(parameter_t *param);

// debug
void parameter_print(parameter_t *param);

#endif /* INCLUDE_BUS_PARAMETER_H_ */
