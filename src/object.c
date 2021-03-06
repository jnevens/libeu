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
	void *userdata;
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
	eu_list_for_each_declare(node, obj->children)
	{
		eu_object_t *child = eu_list_node_data(node);
		if (strcmp(child->name, name) == 0) {
			return child;
		}
	}

	return NULL;
}

eu_object_t *eu_object_get_child_path(eu_object_t *root, const char *path)
{
	char *path_dup = strdup(path);

	char *token;
	eu_object_t *ptr = NULL;
	int n = 0;

	while ((token = strsep(&path_dup, "."))) {
		if (n == 0) {
			if (strcmp(eu_object_name(root), token) == 0) {
				ptr = root;
			} else {
				ptr = NULL;
				goto cleanup;
			}
		} else {
			ptr = eu_object_get_child(ptr, token);
			if (!ptr) {
				ptr = NULL;
				goto cleanup;
			}
		}
		n++;
	}

cleanup:
	free(path_dup);
	return ptr;
}

void eu_object_destroy_child(eu_object_t *obj, const char *name)
{
	eu_list_for_each_declare(node, obj->children)
	{
		eu_object_t *child = eu_list_node_data(node);
		if (strcmp(child->name, name) == 0) {
			eu_list_remove_node(obj->children, node);
			eu_object_destroy(child);
			break;
		}
	}
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

// userdata
void eu_object_set_userdata(eu_object_t *obj, void *userdata)
{
	obj->userdata = userdata;
}

void *eu_object_get_userdata(eu_object_t *obj)
{
	return obj->userdata;
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
	if (!param) {
		param = eu_parameter_create(obj, name, EU_PARAMETER_TYPE_BOOL);
	}
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_BOOL);
	if (eu_variant_set_bool(var, value)) {
		if (eu_parameter_set_value(param, var)) {
			rv = true;
		}
	}
	eu_variant_destroy(var);

	return rv;
}

bool eu_object_parameter_set_int32(eu_object_t *obj, const char *name, int32_t value)
{
	bool rv = false;
	eu_parameter_t *param = eu_object_get_parameter(obj, name);
	if (!param) {
		param = eu_parameter_create(obj, name, EU_PARAMETER_TYPE_INT32);
	}
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_INT32);
	if (eu_variant_set_int32(var, value)) {
		if (eu_parameter_set_value(param, var)) {
			rv = true;
		}
	}
	eu_variant_destroy(var);

	return rv;
}

bool eu_object_parameter_set_float(eu_object_t *obj, const char *name, float value)
{
	bool rv = false;
	eu_parameter_t *param = eu_object_get_parameter(obj, name);
	if (!param) {
		param = eu_parameter_create(obj, name, EU_PARAMETER_TYPE_FLOAT);
	}
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_FLOAT);
	if (eu_variant_set_float(var, value)) {
		if (eu_parameter_set_value(param, var)) {
			rv = true;
		}
	}
	eu_variant_destroy(var);

	return rv;
}

bool eu_object_parameter_set_double(eu_object_t *obj, const char *name, double value)
{
	bool rv = false;
	eu_parameter_t *param = eu_object_get_parameter(obj, name);
	if (!param) {
		param = eu_parameter_create(obj, name, EU_PARAMETER_TYPE_DOUBLE);
	}
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_DOUBLE);
	if (eu_variant_set_double(var, value)) {
		if (eu_parameter_set_value(param, var)) {
			rv = true;
		}
	}
	eu_variant_destroy(var);

	return rv;
}

bool eu_object_parameter_set_char(eu_object_t *obj, const char *name, const char * value)
{
	bool rv = false;
	eu_parameter_t *param = eu_object_get_parameter(obj, name);
	if (!param) {
		param = eu_parameter_create(obj, name, EU_PARAMETER_TYPE_STRING);
	}
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_CHAR);
	if (eu_variant_set_char(var, value)) {
		if (eu_parameter_set_value(param, var)) {
			rv = true;
		}
	}
	eu_variant_destroy(var);

	return rv;
}


// extra
bool eu_object_validate_name(const char *name)
{
	size_t i,j;
	static const char *valid_chars = EU_OBJECT_NAME_VALID_CHARS;

	if(!name || strlen(name) == 0)
		return false;

	size_t len = strlen(name);
	size_t lenc = strlen(valid_chars);

	for(i = 0; i < len; i++) {
		for (j = 0; j < lenc; j++) {
			if (name[i] == valid_chars[j]) {
				break;
			}
		}

		if (j == lenc) { /* char not in valid_chars */
			return false;
		}
	}

	return true;
}

json_object *eu_object_serialize(eu_object_t *obj, uint32_t attrs)
{
	eu_list_node_t *node = NULL;

	if (!obj) {
		return NULL;
	}

	json_object *jobj = json_object_new_object();
	// object parameters (name, isTemplate, ...)
	json_object_object_add(jobj, "name", json_object_new_string(eu_object_name(obj)));
	json_object_object_add(jobj, "isTemplate", json_object_new_boolean(obj->attrs & eu_object_attr_template));
	json_object_object_add(jobj, "isInstance", json_object_new_boolean(obj->attrs & eu_object_attr_instance));

	// children
	json_object *jobj_children = json_object_new_array();
	eu_list_for_each(node, obj->children)
	{
		eu_object_t *child = eu_list_node_data(node);
		json_object *jobj_child = eu_object_serialize(child, attrs);
		json_object_array_add(jobj_children, jobj_child);
	}
	json_object_object_add(jobj, "children", jobj_children);

	// parameters
	json_object *jobj_params = json_object_new_array();
	eu_list_for_each(node, obj->parameters)
	{
		eu_parameter_t *param = eu_list_node_data(node);
		json_object *jobj_param = eu_parameter_serialize(param);
		json_object_array_add(jobj_params, jobj_param);
	}
	json_object_object_add(jobj, "parameters", jobj_params);
	return jobj;
}

eu_object_t *eu_object_deserialize(eu_object_t *parent, json_object *jobj)
{
	eu_object_t *obj = NULL;

	json_object *jobj_name = json_object_object_get(jobj, "name");
	json_object *jobj_isTemplate = json_object_object_get(jobj, "isTemplate");
	json_object *jobj_isInstance = json_object_object_get(jobj, "isInstance");
	json_object *jobj_children = json_object_object_get(jobj, "children");
	json_object *jobj_parameters = json_object_object_get(jobj, "parameters");

	if (json_object_get_string(jobj_name) != NULL) {
		const char *obj_name = json_object_get_string(jobj_name);
		bool isTemplate = json_object_get_boolean(jobj_isTemplate);
		bool isInstance = json_object_get_boolean(jobj_isInstance);
		uint32_t attrs = (isTemplate) ? eu_object_attr_template : eu_object_attr_none;
		attrs |= (isInstance) ? eu_object_attr_instance : eu_object_attr_none;
		obj = eu_object_create(parent, json_object_get_string(jobj_name), attrs);

		int child_count = json_object_array_length(jobj_children);
		for (int i = 0; i < child_count; i++) {
			json_object *child = json_object_array_get_idx(jobj_children, i);
			eu_object_deserialize(obj, child);
		}

		int param_count = json_object_array_length(jobj_parameters);
		for (int i = 0; i < param_count; i++) {
			json_object *param = json_object_array_get_idx(jobj_parameters, i);
			eu_parameter_deserialize(obj, param);
		}
	}

	return obj;
}

void eu_object_print_path(eu_object_t *obj, uint32_t attrs)
{
	if(obj->parent) {
		eu_object_print_path(obj->parent, attrs);
		printf(".");
	}

	if (!(attrs & eu_object_print_attrs_no_root) || obj->parent) {
		printf("%s", obj->name);
		obj = obj->parent;
	}
}

void eu_object_print(eu_object_t *obj, uint32_t attrs)
{
	eu_list_node_t *node;
	if (!obj) {
		return;
	}

	if (!(attrs & eu_object_print_attrs_no_root) || (obj->parent != NULL)) {
		eu_object_print_path(obj, attrs);
		printf("\n");
	}

	if (!(obj->attrs & eu_object_attr_template)) {
		eu_list_for_each(node, obj->parameters) {
			eu_parameter_t *param = eu_list_node_data(node);
			eu_parameter_print(param);
		}
	}
	eu_list_for_each(node, obj->children) {
		eu_object_t *child = eu_list_node_data(node);
		eu_object_print(child, attrs);
	}

}
