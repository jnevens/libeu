#ifndef __EU_OBJECT_H
#define __EU_OBJECT_H

#include <stdbool.h>

#include "eu/types.h"


// object tree
eu_object_t *eu_object_create(eu_object_t *parent, const char *name);
eu_object_t *eu_object_create_path(eu_object_t *root, const char *path);
eu_object_t *eu_object_get_parent(eu_object_t *obj);
eu_object_t *eu_object_get_child(eu_object_t *obj, const char *name);
eu_object_t *eu_object_get_first_child(eu_object_t *obj);
eu_object_t *eu_object_get_last_child(eu_object_t *obj);
eu_object_t *eu_object_get_next_child(eu_object_t *obj);
eu_object_t *eu_object_get_previous_child(eu_object_t *obj);
size_t eu_object_children_count(eu_object_t *obj);
const char *eu_object_name(eu_object_t *obj);
void eu_object_destroy(eu_object_t *obj);

// parameters
bool eu_object_add_parameter(eu_object_t *obj, eu_parameter_t *param);
bool eu_object_remove_parameter(eu_object_t *obj, eu_parameter_t *param);
eu_parameter_t *eu_object_get_parameter(eu_object_t *obj, const char *name);
size_t eu_object_get_parameter_count(eu_object_t *obj);
bool eu_object_has_parameter(eu_object_t *obj, const char *name);
bool eu_object_has_parameters(eu_object_t *obj);

// set value
bool eu_object_parameter_set_bool(eu_object_t *obj, const char *name, bool value);


// debug
void eu_object_print(eu_object_t *obj);
void eu_object_print_path(eu_object_t *obj);

#endif /* __EU_OBJECT_H */
