#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eu/object.h"
#include "eu/list.h"
#include "eu/parameter.h"
#include "eu/variant.h"

struct object_s
{
	struct object_s *parent;
	char *name;
	size_t child_counter;
	list_t *children;
	list_t *parameters;
};

static bool object_add_child(object_t *obj, object_t *child)
{
	list_append(obj->children, child);
	return true;
}

// TODO: if name is NULL, generate name (starting "1", "2", etc...)
object_t *object_create(object_t *parent, const char *name)
{
	object_t *obj = calloc(1, sizeof(object_t));
	obj->name = strdup(name);
	obj->children = list_create();
	obj->parameters = list_create();
	if (parent != NULL) {
		obj->parent = parent;
		object_add_child(parent, obj);
	}
	return obj;
}

const char *object_name(object_t *obj)
{
	return obj->name;
}

object_t *object_get_parent(object_t *obj)
{
	return obj->parent;
}

object_t *object_get_child(object_t *obj, const char *name)
{
	list_node_t *node = NULL;

	list_for_each(node, obj->children)
	{
		object_t *child = list_node_data(node);
		if (strcmp(child->name, name) == 0) {
			return child;
		}
	}

	return NULL;
}

size_t object_children_count(object_t *obj)
{
	return list_count(obj->children);
}

object_t *object_get_first_child(object_t *obj)
{
	obj->child_counter = 0;
	list_node_t *node = list_first(obj->children);
	return (node) ? list_node_data(node) : NULL;
}

object_t *object_get_last_child(object_t *obj)
{
	obj->child_counter = list_count(obj->children) - 1;
	list_node_t *node = list_last(obj->children);
	return (node) ? list_node_data(node) : NULL;
}

object_t *object_get_next_child(object_t *obj)
{
	size_t count = 0;
	list_node_t *node = NULL;
	obj->child_counter++;
	list_for_each(node, obj->children)
	{
		if (obj->child_counter == count)
			return (object_t *) list_node_data(node);
		count++;
	}
	return NULL;
}

object_t *object_get_previous_child(object_t *obj)
{
	size_t count = 0;
	list_node_t *node = NULL;
	obj->child_counter--;
	list_for_each(node, obj->children)
	{
		if (obj->child_counter == count)
			return (object_t *) list_node_data(node);
		count++;
	}
	return NULL;
}

void object_destroy(object_t *obj)
{
	free(obj->name);
	list_destroy_with_data(obj->children, (list_destroy_element_fn_t) object_destroy);
	list_destroy_with_data(obj->parameters, (list_destroy_element_fn_t) parameter_destroy);
	free(obj);
}

// parameters
bool object_add_parameter(object_t *obj, parameter_t *param)
{
	if(!obj || !param) {
		return false;
	}
	list_append(obj->parameters, param);
	return true;
}

bool object_remove_parameter(object_t *obj, parameter_t *param)
{
	return false;
}

parameter_t *object_get_parameter(object_t *obj, const char *name)
{
	list_node_t *node = NULL;

	if (obj && name) {
		list_for_each(node, obj->parameters)
		{
			parameter_t *parameter = list_node_data(node);
			if (strcmp(parameter_name(parameter), name) == 0)
				return parameter;
		}
	}
	return NULL;
}

size_t object_get_parameter_count(object_t *obj)
{
	if (!obj) {
		return 0;
	}

	return list_count(obj->parameters);
}

bool object_has_parameter(object_t *obj, const char *name)
{
	list_node_t *node = NULL;

	if (!obj || !name) {
		return false;
	}

	if (obj && name) {
		list_for_each(node, obj->parameters)
		{
			parameter_t *parameter = list_node_data(node);
			if (strcmp(parameter_name(parameter), name) == 0)
				return true;
		}
	}
	return false;
}

bool object_has_parameters(object_t *obj)
{
	if (!obj) {
		return false;
	}

	return (list_count(obj->parameters)) ? true : false;
}

bool object_parameter_set_bool(object_t *obj, const char *name, bool value)
{
	bool rv = false;
	parameter_t *param = object_get_parameter(obj, name);
	if (param) {
		variant_t *var = variant_create(VARIANT_TYPE_BOOL);
		if (variant_set_bool(var, value)) {
			if (parameter_set_value(param, var)) {
				rv = true;
			}
		}
		variant_destroy(var);
	}

	return rv;
}

void object_print_path(object_t *obj)
{
	if(obj->parent) {
		object_print_path(obj->parent);
		printf(".");
	}
	printf("%s", obj->name);
	obj = obj->parent;
}

void object_print(object_t *obj)
{
	list_node_t *node;
	if (!obj) {
		return;
	}

	object_print_path(obj);
	printf("\n");

	list_for_each(node, obj->parameters) {
		parameter_t *child = list_node_data(node);
		parameter_print(child);
	}
	list_for_each(node, obj->children) {
		object_t *child = list_node_data(node);
		object_print(child);
	}

}
