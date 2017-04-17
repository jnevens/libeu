#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eu/object.h"
#include "eu/list.h"
#include "eu/parameter.h"
#include "eu/variant.h"

struct eu_object_s
{
	struct eu_object_s *parent;
	char *name;
	size_t child_counter;
	eu_list_t *children;
	eu_list_t *parameters;
};

static bool object_add_child(eu_object_t *obj, eu_object_t *child)
{
	eu_list_append(obj->children, child);
	return true;
}

// TODO: if name is NULL, generate name (starting "1", "2", etc...)
eu_object_t *eu_object_create(eu_object_t *parent, const char *name)
{
	eu_object_t *obj = calloc(1, sizeof(eu_object_t));
	obj->name = strdup(name);
	obj->children = eu_list_create();
	obj->parameters = eu_list_create();
	if (parent != NULL) {
		obj->parent = parent;
		object_add_child(parent, obj);
	}
	return obj;
}

const char *eu_object_name(eu_object_t *obj)
{
	return obj->name;
}

eu_object_t *eu_object_get_parent(eu_object_t *obj)
{
	return obj->parent;
}

eu_object_t *eu_object_get_child(eu_object_t *obj, const char *name)
{
	eu_list_node_t *node = NULL;

	eu_list_for_each(node, obj->children)
	{
		eu_object_t *child = eu_list_node_data(node);
		if (strcmp(child->name, name) == 0) {
			return child;
		}
	}

	return NULL;
}

size_t eu_object_children_count(eu_object_t *obj)
{
	return eu_list_count(obj->children);
}

eu_object_t *eu_object_get_first_child(eu_object_t *obj)
{
	obj->child_counter = 0;
	eu_list_node_t *node = eu_list_first(obj->children);
	return (node) ? eu_list_node_data(node) : NULL;
}

eu_object_t *eu_object_get_last_child(eu_object_t *obj)
{
	obj->child_counter = eu_list_count(obj->children) - 1;
	eu_list_node_t *node = eu_list_last(obj->children);
	return (node) ? eu_list_node_data(node) : NULL;
}

eu_object_t *eu_object_get_next_child(eu_object_t *obj)
{
	size_t count = 0;
	eu_list_node_t *node = NULL;
	obj->child_counter++;
	eu_list_for_each(node, obj->children)
	{
		if (obj->child_counter == count)
			return (eu_object_t *) eu_list_node_data(node);
		count++;
	}
	return NULL;
}

eu_object_t *eu_object_get_previous_child(eu_object_t *obj)
{
	size_t count = 0;
	eu_list_node_t *node = NULL;
	obj->child_counter--;
	eu_list_for_each(node, obj->children)
	{
		if (obj->child_counter == count)
			return (eu_object_t *) eu_list_node_data(node);
		count++;
	}
	return NULL;
}

void eu_object_destroy(eu_object_t *obj)
{
	free(obj->name);
	eu_list_destroy_with_data(obj->children, (eu_list_destroy_element_fn_t) eu_object_destroy);
	eu_list_destroy_with_data(obj->parameters, (eu_list_destroy_element_fn_t) eu_parameter_destroy);
	free(obj);
}

// parameters
bool eu_object_add_parameter(eu_object_t *obj, eu_parameter_t *param)
{
	if(!obj || !param) {
		return false;
	}
	eu_list_append(obj->parameters, param);
	return true;
}

bool eu_object_remove_parameter(eu_object_t *obj, eu_parameter_t *param)
{
	return false;
}

eu_parameter_t *eu_object_get_parameter(eu_object_t *obj, const char *name)
{
	eu_list_node_t *node = NULL;

	if (obj && name) {
		eu_list_for_each(node, obj->parameters)
		{
			eu_parameter_t *parameter = eu_list_node_data(node);
			if (strcmp(eu_parameter_name(parameter), name) == 0)
				return parameter;
		}
	}
	return NULL;
}

size_t eu_object_get_parameter_count(eu_object_t *obj)
{
	if (!obj) {
		return 0;
	}

	return eu_list_count(obj->parameters);
}

bool eu_object_has_parameter(eu_object_t *obj, const char *name)
{
	eu_list_node_t *node = NULL;

	if (!obj || !name) {
		return false;
	}

	if (obj && name) {
		eu_list_for_each(node, obj->parameters)
		{
			eu_parameter_t *parameter = eu_list_node_data(node);
			if (strcmp(eu_parameter_name(parameter), name) == 0)
				return true;
		}
	}
	return false;
}

bool eu_object_has_parameters(eu_object_t *obj)
{
	if (!obj) {
		return false;
	}

	return (eu_list_count(obj->parameters)) ? true : false;
}

bool eu_object_parameter_set_bool(eu_object_t *obj, const char *name, bool value)
{
	bool rv = false;
	eu_parameter_t *param = eu_object_get_parameter(obj, name);
	if (param) {
		eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_BOOL);
		if (eu_variant_set_bool(var, value)) {
			if (eu_parameter_set_value(param, var)) {
				rv = true;
			}
		}
		eu_variant_destroy(var);
	}

	return rv;
}

void eu_object_print_path(eu_object_t *obj)
{
	if(obj->parent) {
		eu_object_print_path(obj->parent);
		printf(".");
	}
	printf("%s", obj->name);
	obj = obj->parent;
}

void eu_object_print(eu_object_t *obj)
{
	eu_list_node_t *node;
	if (!obj) {
		return;
	}

	eu_object_print_path(obj);
	printf("\n");

	eu_list_for_each(node, obj->parameters) {
		eu_parameter_t *child = eu_list_node_data(node);
		eu_parameter_print(child);
	}
	eu_list_for_each(node, obj->children) {
		eu_object_t *child = eu_list_node_data(node);
		eu_object_print(child);
	}

}
