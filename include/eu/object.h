#ifndef __EU_OBJECT_H
#define __EU_OBJECT_H

#include <stdbool.h>
#include <stdint.h>

#include "eu/types.h"

#define eu_object_attr_none			0x0000
#define eu_object_attr_instance		0x0001
#define eu_object_attr_template		0x0002

// loop all child's
#define eu_object_for_each_child(child, object) \
		for(child = eu_object_get_first_child(object); child; child = eu_object_get_next_child(object))

#define eu_object_for_each_child_reverse(child, object) \
		for(child = eu_object_get_last_child(object); child; child = eu_object_get_prev_child(object))

#define eu_object_for_each_child_declare(child, object) \
		for(eu_object_t *child = eu_object_get_first_child(object); child; child = eu_object_get_next_child(object))

#define eu_object_for_each_child_reverse_declare(child, object) \
		for(eu_object_t *child = eu_object_get_last_child(object); child; child = eu_object_get_prev_child(object))

// loop all parameters
#define eu_object_for_each_parameter(param, object) \
		for(param = eu_object_get_first_parameter(object); param; param = eu_object_get_next_parameter(param))

#define eu_object_for_each_parameter_reverse(param, object) \
		for(param = eu_object_get_last_parameter(object); param; param = eu_object_get_prev_parameter(param))

#define eu_object_for_each_parameter_declare(param, object) \
		for(eu_parameter_t *param = eu_object_get_first_parameter(object); param; param = eu_object_get_next_parameter(param))

#define eu_object_for_each_parameter_reverse_declare(param, object) \
		for(eu_parameter_t *param = eu_object_get_last_parameter(object); param; param = eu_object_get_prev_parameter(param))


// loop all instances
#define eu_object_for_each_instance(instance, object) \
		for(instance = eu_object_get_first_instance(object); instance; instance = eu_object_get_next_instance(instance))

#define eu_object_for_each_instance_reverse(instance, object) \
		for(instance = eu_object_get_last_instance(object); instance; instance = eu_object_get_prev_instance(instance))

#define eu_object_for_each_instance_declare(instance, object) \
		for(eu_object_t *instance = eu_object_get_first_instance(object); instance; instance = eu_object_get_next_instance(instance))

#define eu_object_for_each_instance_reverse_declare(instance, object) \
		for(eu_object_t *instance = eu_object_get_last_instance(object); instance; instance = eu_object_get_prev_instance(instance))


// object tree
eu_object_t *eu_object_create(eu_object_t *parent, const char *name, uint32_t attr);
eu_object_t *eu_object_create_path(eu_object_t *root, const char *path);
eu_object_t *eu_object_get_parent(eu_object_t *obj);
eu_object_t *eu_object_get_child(eu_object_t *obj, const char *name);
eu_object_t *eu_object_get_first_child(eu_object_t *obj);
eu_object_t *eu_object_get_last_child(eu_object_t *obj);
eu_object_t *eu_object_get_next_child(eu_object_t *child);
eu_object_t *eu_object_get_prev_child(eu_object_t *child);
size_t eu_object_children_count(eu_object_t *obj);
const char *eu_object_name(eu_object_t *obj);
void eu_object_destroy(eu_object_t *obj);

// template & instances
eu_object_t *eu_object_create_instance(eu_object_t *template, const char *name);
size_t eu_object_instance_count(eu_object_t *template);
eu_object_t *eu_object_get_first_instance(eu_object_t *template);
eu_object_t *eu_object_get_last_instance(eu_object_t *template);
eu_object_t *eu_object_get_next_instance(eu_object_t *instance);
eu_object_t *eu_object_get_prev_instance(eu_object_t *instance);

// parameters
bool eu_object_add_parameter(eu_object_t *obj, eu_parameter_t *param);
bool eu_object_remove_parameter(eu_object_t *obj, eu_parameter_t *param);
eu_parameter_t *eu_object_get_parameter(eu_object_t *obj, const char *name);
eu_parameter_t *eu_object_get_first_parameter(eu_object_t *obj);
eu_parameter_t *eu_object_get_last_parameter(eu_object_t *obj);
eu_parameter_t *eu_object_get_next_parameter(eu_parameter_t *param);
eu_parameter_t *eu_object_get_prev_parameter(eu_parameter_t *param);
size_t eu_object_get_parameter_count(eu_object_t *obj);
bool eu_object_has_parameter(eu_object_t *obj, const char *name);
bool eu_object_has_parameters(eu_object_t *obj);

// set value
bool eu_object_parameter_set_bool(eu_object_t *obj, const char *name, bool value);


// debug
void eu_object_print(eu_object_t *obj);
void eu_object_print_path(eu_object_t *obj);

#endif /* __EU_OBJECT_H */
