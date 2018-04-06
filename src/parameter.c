/*
 * parameter.c
 *
 *  Created on: Apr 10, 2016
 *      Author: jnevens
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eu/object.h"
#include "eu/variant.h"
#include "eu/parameter.h"

struct eu_parameter_s
{
	eu_object_t *parent;
	char *name;
	eu_parameter_type_t type;
	eu_variant_t *value;
};

//static variant_type_t param_type_to_variant_type_conversion[] = {
//		VARIANT_TYPE_INVALID,
//		VARIANT_TYPE_BOOL,
//		VARIANT_TYPE_INT8,
//		VARIANT_TYPE_UINT8,
//		VARIANT_TYPE_INT16,
//		VARIANT_TYPE_UINT16,
//		VARIANT_TYPE_INT32,
//		VARIANT_TYPE_UINT32,
//		VARIANT_TYPE_INT64,
//		VARIANT_TYPE_UINT64,
//		VARIANT_TYPE_FLOAT,
//		VARIANT_TYPE_DOUBLE,
//		VARIANT_TYPE_CHAR,
//};
//
//static variant_type_t parameter_type_to_variant_type(parameter_type_t param_type)
//{
//	return param_type_to_variant_type_conversion[param_type];
//}

eu_parameter_t *eu_parameter_create(eu_object_t *obj, const char *name, eu_parameter_type_t type)
{
	eu_parameter_t *param = calloc(1, sizeof(eu_parameter_t));
	param->name = strdup(name);
	param->type = type;
	param->parent = obj;

	eu_object_add_parameter(obj, param);

	return param;
}

eu_parameter_t *eu_parameter_duplicate(eu_object_t *new_parent, eu_parameter_t *orig)
{
	eu_parameter_t *param = calloc(1, sizeof(eu_parameter_t));
	param->name = strdup(orig->name);
	param->type = orig->type;
	param->parent = new_parent;
	if (orig->value)
		param->value = eu_variant_duplicate(orig->value);

	return param;
}

void eu_parameter_destroy(eu_parameter_t *param)
{
	eu_object_remove_parameter(param->parent, param);
	eu_variant_destroy(param->value);
	free(param->name);
	free(param);
}

const char *eu_parameter_name(eu_parameter_t *param)
{
	if(param) {
		return param->name;
	}

	return NULL;
}

eu_object_t *eu_parameter_parent(eu_parameter_t *param)
{
	return (param) ? param->parent : NULL;
}

bool eu_parameter_set_value(eu_parameter_t *param, eu_variant_t *variant)
{
	if(!param || !variant) {
		return false;
	}

	eu_variant_destroy(param->value);

	param->value = eu_variant_duplicate(variant);

	return (param->value) ? true : false;
}

eu_variant_t *eu_parameter_value(eu_parameter_t *param)
{
	return eu_variant_duplicate(param->value);
}

eu_variant_t *eu_parameter_da_value(eu_parameter_t *param)
{
	return param->value;
}

json_object *eu_parameter_serialize(eu_parameter_t *param)
{
	json_object *obj = json_object_new_object();
	json_object_object_add(obj, "name", json_object_new_string(eu_parameter_name(param)));
	json_object_object_add(obj, "type", json_object_new_int((int)param->type));
	json_object_object_add(obj, "value", eu_variant_serialize(eu_parameter_da_value(param)));
	return obj;
}

eu_parameter_t *eu_parameter_deserialize(eu_object_t *obj, json_object *jobj)
{
	json_object *jobj_name = json_object_object_get(jobj, "name");
	json_object *jobj_type = json_object_object_get(jobj, "type");
	json_object *jobj_value = json_object_object_get(jobj, "value");

	eu_parameter_type_t type = json_object_get_int(jobj_type);
	const char *name = json_object_get_string(jobj_name);

	eu_parameter_t *param = eu_parameter_create(obj, name, type);
	eu_variant_t *var = eu_variant_deserialize(jobj_value);
	eu_parameter_set_value(param, var);

	return param;
}

void eu_parameter_print(eu_parameter_t *param)
{
	eu_object_print_path(param->parent, eu_object_print_attrs_none);
	printf(".%s=", param->name);
	if (param->value) {
		eu_variant_print(param->value);
	}
	printf("\n");
}
