#ifndef __BUS_OBJECT_H
#define __BUS_OBJECT_H

#include <stdbool.h>

#include "eu/types.h"


// object tree
object_t *object_create(object_t *parent, const char *name);
object_t *object_get_parent(object_t *obj);
object_t *object_get_child(object_t *obj, const char *name);
object_t *object_get_first_child(object_t *obj);
object_t *object_get_last_child(object_t *obj);
object_t *object_get_next_child(object_t *obj);
object_t *object_get_previous_child(object_t *obj);
size_t object_children_count(object_t *obj);
const char *object_name(object_t *obj);
void object_destroy(object_t *obj);

// parameters
bool object_add_parameter(object_t *obj, parameter_t *param);
bool object_remove_parameter(object_t *obj, parameter_t *param);
parameter_t *object_get_parameter(object_t *obj, const char *name);
size_t object_get_parameter_count(object_t *obj);
bool object_has_parameter(object_t *obj, const char *name);
bool object_has_parameters(object_t *obj);

// set value
bool object_parameter_set_bool(object_t *obj, const char *name, bool value);


// debug
void object_print(object_t *obj);
void object_print_path(object_t *obj);

#endif /* __BUS_OBJECT_H */
