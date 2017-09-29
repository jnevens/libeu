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

struct variant_map_item_s {
	eu_variant_t *value;
	char *key;
};
typedef struct variant_map_item_s variant_map_item_t;

struct eu_variant_map_s {
	eu_list_t *items;
};

static variant_map_item_t *variant_map_item_create(const char *key, eu_variant_t *var)
{
	variant_map_item_t *item = calloc(1, sizeof(variant_map_item_t));
	item->key = strdup(key);
	item->value = var;
	return item;
}

static void variant_map_item_destroy(void *arg)
{
	variant_map_item_t *item = arg;
	eu_variant_destroy(item->value);
	free(item->key);
	free(item);
}

eu_variant_map_t *variant_map_create(void)
{
	eu_variant_map_t *map = calloc(1, sizeof(eu_variant_map_t));
	map->items = eu_list_create();
	return map;
}

void eu_variant_map_destroy(eu_variant_map_t *map)
{
	eu_list_destroy_with_data(map->items, variant_map_item_destroy);
	free(map);
}

static variant_map_item_t *variant_map_item_find(eu_variant_map_t *map, const char *key) {
	eu_list_node_t *iter = NULL;

	if (!map || !key) {
		return NULL;
	}

	eu_list_for_each(iter, map->items)
	{
		variant_map_item_t *item = eu_list_node_data(iter);
		if (strcmp(item->key, key) == 0) {
			return item;
		}
	}
	return NULL;
}

bool eu_variant_map_set_variant(eu_variant_map_t *map, const char *key, eu_variant_t *var)
{
	if (!map || !key || !var) {
		eu_variant_destroy(var);
		return false;
	}

	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		eu_variant_destroy(item->value);
		item->value = var;
	} else {
		variant_map_item_t *item = variant_map_item_create(key, var);
		eu_list_append(map->items, item);
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
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return item->value;
	}
	return NULL;
}

bool eu_variant_map_get_bool(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return eu_variant_bool(item->value);
	}
	return false;
}

int8_t eu_variant_map_get_int8(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return eu_variant_int8(item->value);
	}
	return -1;
}

uint8_t eu_variant_map_get_uint8(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return eu_variant_uint8(item->value);
	}
	return -1;
}

int16_t eu_variant_map_get_int16(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return eu_variant_int16(item->value);
	}
	return -1;
}

uint16_t eu_variant_map_get_uint16(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return eu_variant_uint16(item->value);
	}
	return -1;
}

int32_t eu_variant_map_get_int32(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return eu_variant_int32(item->value);
	}
	return -1;
}

uint32_t eu_variant_map_get_uint32(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return eu_variant_uint32(item->value);
	}
	return -1;
}

int64_t eu_variant_map_get_int64(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return eu_variant_int64(item->value);
	}
	return -1;
}

uint64_t eu_variant_map_get_uint64(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return eu_variant_uint64(item->value);
	}
	return -1;
}

float eu_variant_map_get_float(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return eu_variant_float(item->value);
	}
	return 0.0/0.0;
}

double eu_variant_map_get_double(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return eu_variant_double(item->value);
	}
	return 0.0/0.0;
}

char *eu_variant_map_get_char(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return strdup(eu_variant_da_char(item->value));
	}
	return NULL;
}

eu_string_t *eu_variant_map_get_string(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return eu_variant_da_string(item->value);
	}
	return NULL;
}


const eu_variant_t *eu_variant_map_find(eu_variant_map_t *map, const char *key)
{
	eu_list_node_t *iter = NULL;
	eu_list_for_each(iter, map->items)
	{
		variant_map_item_t *item = eu_list_node_data(iter);
		if (strcmp(item->key, key) == 0) {
			return item->value;
		}
	}
	return NULL;
}

bool eu_variant_map_delete(eu_variant_map_t *map, const char *key)
{
	eu_list_node_t *iter = NULL;
	eu_list_for_each(iter, map->items)
	{
		variant_map_item_t *item = eu_list_node_data(iter);
		if (strcmp(item->key, key) == 0) {
			variant_map_item_t *item = eu_list_node_data(iter);
			variant_map_item_destroy(item);
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
		variant_map_item_t *item = eu_list_node_data(iter);
		if (strcmp(item->key, key) == 0) {
			variant_map_item_t *item = eu_list_node_data(iter);
			var = item->value;
			item->value = NULL;
			variant_map_item_destroy(item);
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

