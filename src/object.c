/*
 * object.c
 *
 *  Created on: Apr 10, 2016
 *      Author: jnevens
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eu/list.h"
#include "eu/parameter.h"
#include "eu/variant.h"
#include "eu/log.h"
#include "eu/object.h"

struct eu_object_s
{
	struct eu_object_s *parent;
	char *name;
	uint32_t attrs;
	eu_list_t *children;
	eu_list_t *parameters;
};

static bool object_add_child(eu_object_t *obj, eu_object_t *child)
{
	eu_list_append(obj->children, child);
	return true;
}

eu_object_t *eu_object_create(eu_object_t *parent, const char *name, uint32_t attrs)
{
	if (name == NULL)
		return NULL;

	if (parent && (parent->attrs & eu_object_attr_template) && !(attrs & eu_object_attr_instance)) {
		eu_log_err("cannot create child of template! create instance!");
		return NULL;
	}

	if ((attrs & eu_object_attr_instance) && parent == NULL) {
		eu_log_err("can only create instance if parent object exist and is a template!");
		return NULL;
	}

	if ((attrs & eu_object_attr_instance) && !(parent->attrs & eu_object_attr_template)) {
		eu_log_err("can only create instance if parent object is a template!");
		return NULL;
	}

	eu_object_t *obj = calloc(1, sizeof(eu_object_t));
	obj->name = strdup(name);
	obj->children = eu_list_create();
	obj->parameters = eu_list_create();
	obj->attrs = attrs;
	if (parent != NULL) {
		obj->parent = parent;
		object_add_child(parent, obj);
	}
	return obj;
}

eu_object_t *eu_object_create_path(eu_object_t *root, const char *path)
{
	char *path_dup = strdup(path);
	char *path_dup_ptr = path_dup;
	char *token;
	int n = 0;
	eu_object_t *ptr = NULL;

	while ((token = strsep(&path_dup, ".")))
	{
		if (root == NULL) {
			root = eu_object_create(NULL, token, eu_object_attr_none);
			ptr = root;
		} else if (!ptr) {
			if (strcmp(eu_object_name(root), token) != 0) {
				// problem, create new root ?
			}
			ptr = root;
		} else {
			eu_object_t *child = eu_object_get_child(ptr, token);
			if (!child) {
				child = eu_object_create(ptr, token, eu_object_attr_none);
			}
			ptr = child;
		}
		n++;
	}

	free(path_dup_ptr);
	return root;
}

const char *eu_object_name(eu_object_t *obj)
{
	if (!obj) {
		return NULL;
	}

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
	eu_list_node_t *node = eu_list_first(obj->children);
	return (node) ? eu_list_node_data(node) : NULL;
}

eu_object_t *eu_object_get_last_child(eu_object_t *obj)
{
	eu_list_node_t *node = eu_list_last(obj->children);
	return (node) ? eu_list_node_data(node) : NULL;
}

eu_object_t *eu_object_get_next_child(eu_object_t *child) // TODO: fix loop over all children
{
	eu_list_for_each_declare(node, child->parent->children)
	{
		if (eu_list_node_data(node) == child) {
			eu_list_node_t *node_next = eu_list_node_next(node);
			return (node_next) ? eu_list_node_data(node_next) : NULL;
		}
	}
	return NULL;
}

eu_object_t *eu_object_get_prev_child(eu_object_t *child) // TODO, fix loop over all children
{
	eu_list_for_each_declare(node, child->parent->children)
	{
		if (eu_list_node_data(node) == child) {
			eu_list_node_t *node_prev = eu_list_node_prev(node);
			return (node_prev) ? eu_list_node_data(node_prev) : NULL;
		}
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

// template & instances
eu_object_t *eu_object_create_instance(eu_object_t *tmpl, const char *name)
{
	if (!(tmpl->attrs & eu_object_attr_template)) {
		eu_log_err("parent is not template object!");
		return NULL;
	}

	eu_object_t *instance = eu_object_create(tmpl, name, eu_object_attr_instance);
	if (!instance) {
		return NULL;
	}

	if (name == NULL) {
		asprintf(&instance->name, "%zu", eu_object_instance_count(tmpl));
	}

	// Copy all parameters
	eu_object_for_each_parameter_declare(param_orig, tmpl) {
		eu_parameter_t *param_new = eu_parameter_duplicate(instance, param_orig);
		eu_object_add_parameter(instance, param_new);
	}

	return instance;
}

size_t eu_object_instance_count(eu_object_t *template)
{
	return eu_object_children_count(template);
}

eu_object_t *eu_object_get_first_instance(eu_object_t *template)
{
	return eu_object_get_first_child(template);
}

eu_object_t *eu_object_get_last_instance(eu_object_t *template)
{
	return eu_object_get_last_child(template);
}

eu_object_t *eu_object_get_next_instance(eu_object_t *instance)
{
	return eu_object_get_next_child(instance);
}

eu_object_t *eu_object_get_prev_instance(eu_object_t *instance)
{
	return eu_object_get_prev_child(instance);
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

eu_parameter_t *eu_object_get_first_parameter(eu_object_t *obj)
{
	eu_list_node_t *node = eu_list_first(obj->parameters);
	return (node) ? eu_list_node_data(node) : NULL;
}

eu_parameter_t *eu_object_get_last_parameter(eu_object_t *obj)
{
	eu_list_node_t *node = eu_list_last(obj->parameters);
	return (node) ? eu_list_node_data(node) : NULL;
}

eu_parameter_t *eu_object_get_next_parameter(eu_parameter_t *param) // TODO: fix loop over all children
{
	eu_list_for_each_declare(node, eu_parameter_parent(param)->parameters)
	{
		eu_parameter_t *cparam = eu_list_node_data(node);
		if (strcmp(eu_parameter_name(cparam), eu_parameter_name(param)) == 0) {
			eu_list_node_t *node_next = eu_list_node_next(node);

			return (node_next) ? eu_list_node_data(node_next) : NULL;
		}
	}

	return NULL;
}

eu_parameter_t *eu_object_get_prev_parameter(eu_parameter_t *param) // TODO: fix loop over all children
{
	eu_list_for_each_reverse_declare(node, eu_parameter_parent(param)->parameters)
	{
		eu_parameter_t *cparam = eu_list_node_data(node);
		if (strcmp(eu_parameter_name(cparam), eu_parameter_name(param)) == 0) {
			eu_list_node_t *node_prev = eu_list_node_prev(node);

			return (node_prev) ? eu_list_node_data(node_prev) : NULL;
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

	if (!(obj->attrs & eu_object_attr_template)) {
		eu_list_for_each(node, obj->parameters) {
			eu_parameter_t *param = eu_list_node_data(node);
			eu_parameter_print(param);
		}
	}
	eu_list_for_each(node, obj->children) {
		eu_object_t *child = eu_list_node_data(node);
		eu_object_print(child);
	}

}
