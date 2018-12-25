/*
 * variant_map.c
 *
 *  Created on: Nov 9, 2016
 *      Author: jnevens
 */
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <eu/list.h>
#include <eu/variant.h>
#include <eu/variant_map.h>

struct eu_variant_map_s
{
	eu_list_t *items;
};

struct eu_variant_map_pair_s
{
	eu_variant_map_t *map;
	eu_list_node_t *list_node;
	char *key;
	eu_variant_t *value;
};

/* variant pair */
eu_variant_map_pair_t *eu_variant_map_pair_create(const char *key, eu_variant_t *var)
{
	eu_variant_map_pair_t *item = calloc(1, sizeof(eu_variant_map_pair_t));
	item->key = strdup(key);
	item->value = var;
	return item;
}

void eu_variant_map_pair_destroy(void *arg)
{
	eu_variant_map_pair_t *item = arg;
	eu_variant_destroy(item->value);
	free(item->key);
	free(item);
}

const char *eu_variant_map_pair_get_key(eu_variant_map_pair_t *map_item)
{
	return map_item->key;
}

eu_variant_t *eu_variant_map_pair_get_val(eu_variant_map_pair_t *map_item)
{
	return map_item->value;
}

eu_variant_map_t *eu_variant_map_pair_get_map(eu_variant_map_pair_t *map_item)
{
	return map_item->map;
}

/* variant map */
eu_variant_map_t *eu_variant_map_create(void)
{
	eu_variant_map_t *map = calloc(1, sizeof(eu_variant_map_t));
	if (map) {
		map->items = eu_list_create();
	}
	return map;
}

void eu_variant_map_destroy(eu_variant_map_t *map)
{
	eu_list_destroy_with_data(map->items, eu_variant_map_pair_destroy);
	free(map);
}

eu_variant_map_pair_t *eu_variant_map_get_first(eu_variant_map_t *map)
{
	eu_list_node_t *node_first = eu_list_first(map->items);
	if (node_first ) {
		return (eu_variant_map_pair_t *)eu_list_node_data(node_first);
	}

	return NULL;
}

eu_variant_map_pair_t *eu_variant_map_get_last(eu_variant_map_t *map)
{
	eu_list_node_t *node_last = eu_list_last(map->items);
	if (node_last) {
		return (eu_variant_map_pair_t *)eu_list_node_data(node_last);
	}

	return NULL;
}

eu_variant_map_pair_t *eu_variant_map_get_next(eu_variant_map_pair_t *map_item)
{
	eu_list_node_t *node_next = eu_list_node_next(map_item->list_node);
	if (node_next) {
		return (eu_variant_map_pair_t *)eu_list_node_data(node_next);
	}

	return NULL;
}

eu_variant_map_pair_t *eu_variant_map_get_prev(eu_variant_map_pair_t *map_item)
{
	eu_list_node_t *node_prev = eu_list_node_prev(map_item->list_node);
	if (node_prev) {
		return (eu_variant_map_pair_t *)eu_list_node_data(node_prev);
	}

	return NULL;
}

eu_variant_map_pair_t *eu_variant_map_find_pair(eu_variant_map_t *map, const char *key)
{
	eu_list_node_t *iter = NULL;
	eu_list_for_each(iter, map->items)
	{
		eu_variant_map_pair_t *pair = eu_list_node_data(iter);
		if (strcmp(pair->key, key) == 0) {
			return pair;
		}
	}
	return NULL;
}

eu_variant_t *eu_variant_map_find(eu_variant_map_t *map, const char *key)
{
	eu_variant_map_pair_t *pair = eu_variant_map_find_pair(map, key);
	return (pair) ? eu_variant_map_pair_get_val(pair) : NULL;
}

bool eu_variant_map_has(eu_variant_map_t *map, const char *key)
{
	return (eu_variant_map_find_pair(map, key) != NULL) ? true : false;
}

bool eu_variant_map_delete(eu_variant_map_t *map, const char *key)
{
	eu_list_node_t *iter = NULL;
	eu_list_for_each(iter, map->items)
	{
		eu_variant_map_pair_t *item = eu_list_node_data(iter);
		if (strcmp(item->key, key) == 0) {
			eu_variant_map_pair_t *item = eu_list_node_data(iter);
			eu_variant_map_pair_destroy(item);
			eu_list_remove_node(map->items, iter);
			return true;
		}
	}

	return false;
}

eu_variant_t *eu_variant_map_take(eu_variant_map_t *map, const char *key)
{
	eu_variant_t *var = NULL;
	eu_list_node_t *iter = NULL;
	eu_list_for_each(iter, map->items)
	{
		eu_variant_map_pair_t *item = eu_list_node_data(iter);
		if (strcmp(item->key, key) == 0) {
			eu_variant_map_pair_t *item = eu_list_node_data(iter);
			var = item->value;
			item->value = NULL;
			eu_variant_map_pair_destroy(item);
			eu_list_remove_node(map->items, iter);
			break;
		}
	}

	return var;
}

size_t eu_variant_map_count(eu_variant_map_t *map)
{
	return eu_list_count(map->items);
}

bool eu_variant_map_is_empty(eu_variant_map_t *map)
{
	return (eu_list_count(map->items) > 0) ?  false : true;
}

void eu_variant_map_add_pair(eu_variant_map_t *map, eu_variant_map_pair_t *pair)
{
	if (!map || !pair)
		return;

	pair->map = map;
	eu_list_node_t *node = eu_list_append(map->items, pair);
	pair->list_node = node;
}

/* set functions */
bool eu_variant_map_set_variant(eu_variant_map_t *map, const char *key, eu_variant_t *var)
{
	if (!map || !key || !var) {
		eu_variant_destroy(var);
		return false;
	}

	eu_variant_map_pair_t *pair = eu_variant_map_find_pair(map, key);
	if (pair) {
		eu_variant_destroy(pair->value);
		pair->value = var;
	} else {
		eu_variant_map_pair_t *pair = eu_variant_map_pair_create(key, var);
		eu_variant_map_add_pair(map, pair);
	}

	return true;
}

bool eu_variant_map_set_bool(eu_variant_map_t *map, const char *key, bool data)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_BOOL);
	if (!var) {
		return false;
	}
	if (!eu_variant_set_bool(var, data)) {
		eu_variant_destroy(var);
		return false;
	}
	return eu_variant_map_set_variant(map, key, var);
}

bool eu_variant_map_set_int8(eu_variant_map_t *map, const char *key, int8_t data)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_INT8);
	if (!var) {
		return false;
	}
	if (!eu_variant_set_int8(var, data)) {
		eu_variant_destroy(var);
		return false;
	}
	return eu_variant_map_set_variant(map, key, var);
}

bool eu_variant_map_set_uint8(eu_variant_map_t *map, const char *key, uint8_t data)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_UINT8);
	if (!var) {
		return false;
	}
	if (!eu_variant_set_uint8(var, data)) {
		eu_variant_destroy(var);
		return false;
	}
	return eu_variant_map_set_variant(map, key, var);
}

bool eu_variant_map_set_int16(eu_variant_map_t *map, const char *key, int16_t data)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_INT16);
	if (!var) {
		return false;
	}
	if (!eu_variant_set_int16(var, data)) {
		eu_variant_destroy(var);
		return false;
	}
	return eu_variant_map_set_variant(map, key, var);
}

bool eu_variant_map_set_uint16(eu_variant_map_t *map, const char *key, uint16_t data)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_UINT16);
	if (!var) {
		return false;
	}
	if (!eu_variant_set_uint16(var, data)) {
		eu_variant_destroy(var);
		return false;
	}
	return eu_variant_map_set_variant(map, key, var);
}

bool eu_variant_map_set_int32(eu_variant_map_t *map, const char *key, int32_t data)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_INT32);
	if (!var) {
		return false;
	}
	if (!eu_variant_set_int32(var, data)) {
		eu_variant_destroy(var);
		return false;
	}
	return eu_variant_map_set_variant(map, key, var);
}

bool eu_variant_map_set_uint32(eu_variant_map_t *map, const char *key, uint32_t data)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_UINT32);
	if (!var) {
		return false;
	}
	if (!eu_variant_set_uint32(var, data)) {
		eu_variant_destroy(var);
		return false;
	}
	return eu_variant_map_set_variant(map, key, var);
}

bool eu_variant_map_set_int64(eu_variant_map_t *map, const char *key, int64_t data)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_INT64);
	if (!var) {
		return false;
	}
	if (!eu_variant_set_int64(var, data)) {
		eu_variant_destroy(var);
		return false;
	}
	return eu_variant_map_set_variant(map, key, var);
}

bool eu_variant_map_set_uint64(eu_variant_map_t *map, const char *key, uint64_t data)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_UINT64);
	if (!var) {
		return false;
	}
	if (!eu_variant_set_uint64(var, data)) {
		eu_variant_destroy(var);
		return false;
	}
	return eu_variant_map_set_variant(map, key, var);
}

bool eu_variant_map_set_float(eu_variant_map_t *map, const char *key, float data)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_FLOAT);
	if (!var) {
		return false;
	}
	if (!eu_variant_set_float(var, data)) {
		eu_variant_destroy(var);
		return false;
	}
	return eu_variant_map_set_variant(map, key, var);
}

bool eu_variant_map_set_double(eu_variant_map_t *map, const char *key, double data)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_DOUBLE);
	if (!var) {
		return false;
	}
	if (!eu_variant_set_double(var, data)) {
		eu_variant_destroy(var);
		return false;
	}
	return eu_variant_map_set_variant(map, key, var);
}

bool eu_variant_map_set_char(eu_variant_map_t *map, const char *key, const char *data)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_CHAR);
	if (!var) {
		return false;
	}
	if (!eu_variant_set_char(var, data)) {
		eu_variant_destroy(var);
		return false;
	}
	return eu_variant_map_set_variant(map, key, var);
}

bool eu_variant_map_set_string(eu_variant_map_t *map, const char *key, const eu_string_t *data)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_STRING);
	if (!var) {
		return false;
	}
	if (!eu_variant_set_string(var, data)) {
		eu_variant_destroy(var);
		return false;
	}
	return eu_variant_map_set_variant(map, key, var);
}

eu_variant_t *eu_variant_map_get_variant(eu_variant_map_t *map, const char *key)
{
	eu_variant_map_pair_t *item = eu_variant_map_find_pair(map, key);
	if (item) {
		return item->value;
	}
	return NULL;
}

bool eu_variant_map_get_bool(eu_variant_map_t *map, const char *key)
{
	eu_variant_map_pair_t *item = eu_variant_map_find_pair(map, key);
	if (item) {
		return eu_variant_bool(item->value);
	}
	return false;
}

int8_t eu_variant_map_get_int8(eu_variant_map_t *map, const char *key)
{
	eu_variant_map_pair_t *item = eu_variant_map_find_pair(map, key);
	if (item) {
		return eu_variant_int8(item->value);
	}
	return -1;
}

uint8_t eu_variant_map_get_uint8(eu_variant_map_t *map, const char *key)
{
	eu_variant_map_pair_t *item = eu_variant_map_find_pair(map, key);
	if (item) {
		return eu_variant_uint8(item->value);
	}
	return -1;
}

int16_t eu_variant_map_get_int16(eu_variant_map_t *map, const char *key)
{
	eu_variant_map_pair_t *item = eu_variant_map_find_pair(map, key);
	if (item) {
		return eu_variant_int16(item->value);
	}
	return -1;
}

uint16_t eu_variant_map_get_uint16(eu_variant_map_t *map, const char *key)
{
	eu_variant_map_pair_t *item = eu_variant_map_find_pair(map, key);
	if (item) {
		return eu_variant_uint16(item->value);
	}
	return -1;
}

int32_t eu_variant_map_get_int32(eu_variant_map_t *map, const char *key)
{
	eu_variant_map_pair_t *item = eu_variant_map_find_pair(map, key);
	if (item) {
		return eu_variant_int32(item->value);
	}
	return -1;
}

uint32_t eu_variant_map_get_uint32(eu_variant_map_t *map, const char *key)
{
	eu_variant_map_pair_t *item = eu_variant_map_find_pair(map, key);
	if (item) {
		return eu_variant_uint32(item->value);
	}
	return -1;
}

int64_t eu_variant_map_get_int64(eu_variant_map_t *map, const char *key)
{
	eu_variant_map_pair_t *item = eu_variant_map_find_pair(map, key);
	if (item) {
		return eu_variant_int64(item->value);
	}
	return -1;
}

uint64_t eu_variant_map_get_uint64(eu_variant_map_t *map, const char *key)
{
	eu_variant_map_pair_t *item = eu_variant_map_find_pair(map, key);
	if (item) {
		return eu_variant_uint64(item->value);
	}
	return -1;
}

float eu_variant_map_get_float(eu_variant_map_t *map, const char *key)
{
	eu_variant_map_pair_t *item = eu_variant_map_find_pair(map, key);
	if (item) {
		return eu_variant_float(item->value);
	}
	return 0.0/0.0;
}

double eu_variant_map_get_double(eu_variant_map_t *map, const char *key)
{
	eu_variant_map_pair_t *item = eu_variant_map_find_pair(map, key);
	if (item) {
		return eu_variant_double(item->value);
	}
	return 0.0/0.0;
}

char *eu_variant_map_get_char(eu_variant_map_t *map, const char *key)
{
	eu_variant_map_pair_t *item = eu_variant_map_find_pair(map, key);
	if (item) {
		return strdup(eu_variant_da_char(item->value));
	}
	return NULL;
}

eu_string_t *eu_variant_map_get_string(eu_variant_map_t *map, const char *key)
{
	eu_variant_map_pair_t *item = eu_variant_map_find_pair(map, key);
	if (item) {
		return eu_variant_da_string(item->value);
	}
	return NULL;
}

json_object *eu_variant_map_serialize(const eu_variant_map_t *map)
{
	json_object *map_obj = json_object_new_array();
	eu_list_node_t *iter = NULL;

	eu_list_for_each(iter, map->items)
	{
		eu_variant_map_pair_t *pair = eu_list_node_data(iter);
		const char *key = eu_variant_map_pair_get_key(pair);
		eu_variant_t *var = eu_variant_map_pair_get_val(pair);

		json_object *map_item = json_object_new_object();
		json_object_object_add(map_item, "name", json_object_new_string(key));
		json_object_object_add(map_item, "value", eu_variant_serialize(var));
		json_object_array_add(map_obj, map_item);
	}

	return map_obj;
}

eu_variant_map_t *eu_variant_map_deserialize(json_object *obj)
{
	eu_variant_map_t *map = eu_variant_map_create();

	int map_count = json_object_array_length(obj);
	for (int i = 0; i < map_count; i++) {
		json_object *map_item = json_object_array_get_idx(obj, i);
		const char *name = json_object_get_string(json_object_object_get(map_item, "name"));
		json_object *map_item_val = json_object_object_get(map_item, "value");
		eu_variant_t *var = eu_variant_deserialize(map_item_val);
		eu_variant_map_set_variant(map, name, var);
	}

	return map;
}
