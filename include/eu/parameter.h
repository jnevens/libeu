/*
 * parameter.h
 *
 *  Created on: Apr 10, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_EU_PARAMETER_H_
#define INCLUDE_EU_PARAMETER_H_

#include "eu/types.h"

typedef enum
{
	EU_PARAMETER_TYPE_INVALID = 0, /* invalid! */
	EU_PARAMETER_TYPE_BOOL,
	EU_PARAMETER_TYPE_INT8,
	EU_PARAMETER_TYPE_UINT8,
	EU_PARAMETER_TYPE_INT16,
	EU_PARAMETER_TYPE_UINT16,
	EU_PARAMETER_TYPE_INT32,
	EU_PARAMETER_TYPE_UINT32,
	EU_PARAMETER_TYPE_INT64,
	EU_PARAMETER_TYPE_UINT64,
	EU_PARAMETER_TYPE_FLOAT,
	EU_PARAMETER_TYPE_DOUBLE,
	EU_PARAMETER_TYPE_STRING,
} eu_parameter_type_t;

// create
eu_parameter_t *eu_parameter_create(eu_object_t *obj, const char *name, eu_parameter_type_t type);
eu_parameter_t *eu_parameter_duplicate(eu_object_t *new_parent, eu_parameter_t *orig);

// destroy
void eu_parameter_destroy(eu_parameter_t *param);

// info
const char *eu_parameter_name(eu_parameter_t *param);
eu_object_t *eu_parameter_parent(eu_parameter_t *param);

// value
bool eu_parameter_set_value(eu_parameter_t *param, eu_variant_t *variant);
eu_variant_t *eu_parameter_value(eu_parameter_t *param);
eu_variant_t *eu_parameter_da_value(eu_parameter_t *param);

// debug
void eu_parameter_print(eu_parameter_t *param);

#endif /* INCLUDE_EU_PARAMETER_H_ */
