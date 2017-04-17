#ifndef __BUS_OBJECT_H
#define __BUS_OBJECT_H

#include <stdbool.h>

#include "eu/types.h"


// object tree
eu_object_t *object_create(eu_object_t *parent, const char *name);
eu_object_t *object_get_parent(eu_object_t *obj);
eu_object_t *object_get_child(eu_object_t *obj, const char *name);
eu_object_t *object_get_first_child(eu_object_t *obj);
eu_object_t *object_get_last_child(eu_object_t *obj);
eu_object_t *object_get_next_child(eu_object_t *obj);
eu_object_t *object_get_previous_child(eu_object_t *obj);
size_t object_children_count(eu_object_t *obj);
const char *object_name(eu_object_t *obj);
void object_destroy(eu_object_t *obj);

// parameters
bool object_add_parameter(eu_object_t *obj, eu_parameter_t *param);
bool object_remove_parameter(eu_object_t *obj, eu_parameter_t *param);
eu_parameter_t *object_get_parameter(eu_object_t *obj, const char *name);
size_t object_get_parameter_count(eu_object_t *obj);
bool object_has_parameter(eu_object_t *obj, const char *name);
bool object_has_parameters(eu_object_t *obj);

// set value
bool object_parameter_set_bool(eu_object_t *obj, const char *name, bool value);


// debug
void object_print(eu_object_t *obj);
void object_print_path(eu_object_t *obj);

#endif /* __BUS_OBJECT_H */
