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
	variant_destroy(item->value);
	free(item->key);
	free(item);
}

eu_variant_map_t *variant_map_create(void)
{
	eu_variant_map_t *map = calloc(1, sizeof(eu_variant_map_t));
	map->items = list_create();
	return map;
}

void variant_map_destroy(eu_variant_map_t *map)
{
	list_destroy_with_data(map->items, variant_map_item_destroy);
	free(map);
}

static variant_map_item_t *variant_map_item_find(eu_variant_map_t *map, const char *key) {
	eu_list_node_t *iter = NULL;

	if (!map || !key) {
		return NULL;
	}

	list_for_each(iter, map->items)
	{
		variant_map_item_t *item = list_node_data(iter);
		if (strcmp(item->key, key) == 0) {
			return item;
		}
	}
	return NULL;
}

bool variant_map_set_variant(eu_variant_map_t *map, const char *key, eu_variant_t *var)
{
	if (!map || !key || !var) {
		variant_destroy(var);
		return false;
	}

	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		variant_destroy(item->value);
		item->value = var;
	} else {
		variant_map_item_t *item = variant_map_item_create(key, var);
		list_append(map->items, item);
	}

	return true;
}

bool variant_map_set_bool(eu_variant_map_t *map, const char *key, bool data)
{
	eu_variant_t *var = variant_create(VARIANT_TYPE_BOOL);
	if (!var) {
		return false;
	}
	if (!variant_set_bool(var, data)) {
		variant_destroy(var);
		return false;
	}
	return variant_map_set_variant(map, key, var);
}

bool variant_map_set_int8(eu_variant_map_t *map, const char *key, int8_t data)
{
	eu_variant_t *var = variant_create(VARIANT_TYPE_INT8);
	if (!var) {
		return false;
	}
	if (!variant_set_int8(var, data)) {
		variant_destroy(var);
		return false;
	}
	return variant_map_set_variant(map, key, var);
}

bool variant_map_set_uint8(eu_variant_map_t *map, const char *key, uint8_t data)
{
	eu_variant_t *var = variant_create(VARIANT_TYPE_UINT8);
	if (!var) {
		return false;
	}
	if (!variant_set_uint8(var, data)) {
		variant_destroy(var);
		return false;
	}
	return variant_map_set_variant(map, key, var);
}

bool variant_map_set_int16(eu_variant_map_t *map, const char *key, int16_t data)
{
	eu_variant_t *var = variant_create(VARIANT_TYPE_INT16);
	if (!var) {
		return false;
	}
	if (!variant_set_int16(var, data)) {
		variant_destroy(var);
		return false;
	}
	return variant_map_set_variant(map, key, var);
}

bool variant_map_set_uint16(eu_variant_map_t *map, const char *key, uint16_t data)
{
	eu_variant_t *var = variant_create(VARIANT_TYPE_UINT16);
	if (!var) {
		return false;
	}
	if (!variant_set_uint16(var, data)) {
		variant_destroy(var);
		return false;
	}
	return variant_map_set_variant(map, key, var);
}

bool variant_map_set_int32(eu_variant_map_t *map, const char *key, int32_t data)
{
	eu_variant_t *var = variant_create(VARIANT_TYPE_INT32);
	if (!var) {
		return false;
	}
	if (!variant_set_int32(var, data)) {
		variant_destroy(var);
		return false;
	}
	return variant_map_set_variant(map, key, var);
}

bool variant_map_set_uint32(eu_variant_map_t *map, const char *key, uint32_t data)
{
	eu_variant_t *var = variant_create(VARIANT_TYPE_UINT32);
	if (!var) {
		return false;
	}
	if (!variant_set_uint32(var, data)) {
		variant_destroy(var);
		return false;
	}
	return variant_map_set_variant(map, key, var);
}

bool variant_map_set_int64(eu_variant_map_t *map, const char *key, int64_t data)
{
	eu_variant_t *var = variant_create(VARIANT_TYPE_INT64);
	if (!var) {
		return false;
	}
	if (!variant_set_int64(var, data)) {
		variant_destroy(var);
		return false;
	}
	return variant_map_set_variant(map, key, var);
}

bool variant_map_set_uint64(eu_variant_map_t *map, const char *key, uint64_t data)
{
	eu_variant_t *var = variant_create(VARIANT_TYPE_UINT64);
	if (!var) {
		return false;
	}
	if (!variant_set_uint64(var, data)) {
		variant_destroy(var);
		return false;
	}
	return variant_map_set_variant(map, key, var);
}

bool variant_map_set_float(eu_variant_map_t *map, const char *key, float data)
{
	eu_variant_t *var = variant_create(VARIANT_TYPE_FLOAT);
	if (!var) {
		return false;
	}
	if (!variant_set_float(var, data)) {
		variant_destroy(var);
		return false;
	}
	return variant_map_set_variant(map, key, var);
}

bool variant_map_set_double(eu_variant_map_t *map, const char *key, double data)
{
	eu_variant_t *var = variant_create(VARIANT_TYPE_DOUBLE);
	if (!var) {
		return false;
	}
	if (!variant_set_double(var, data)) {
		variant_destroy(var);
		return false;
	}
	return variant_map_set_variant(map, key, var);
}

bool variant_map_set_char(eu_variant_map_t *map, const char *key, const char *data)
{
	eu_variant_t *var = variant_create(VARIANT_TYPE_CHAR);
	if (!var) {
		return false;
	}
	if (!variant_set_char(var, data)) {
		variant_destroy(var);
		return false;
	}
	return variant_map_set_variant(map, key, var);
}

bool variant_map_set_string(eu_variant_map_t *map, const char *key, const eu_string_t *data)
{
	eu_variant_t *var = variant_create(VARIANT_TYPE_STRING);
	if (!var) {
		return false;
	}
	if (!variant_set_string(var, data)) {
		variant_destroy(var);
		return false;
	}
	return variant_map_set_variant(map, key, var);
}

eu_variant_t *variant_map_get_variant(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return item->value;
	}
	return NULL;
}

bool variant_map_get_bool(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return variant_bool(item->value);
	}
	return false;
}

int8_t variant_map_get_int8(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return variant_int8(item->value);
	}
	return -1;
}

uint8_t variant_map_get_uint8(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return variant_uint8(item->value);
	}
	return -1;
}

int16_t variant_map_get_int16(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return variant_int16(item->value);
	}
	return -1;
}

uint16_t variant_map_get_uint16(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return variant_uint16(item->value);
	}
	return -1;
}

int32_t variant_map_get_int32(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return variant_int32(item->value);
	}
	return -1;
}

uint32_t variant_map_get_uint32(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return variant_uint32(item->value);
	}
	return -1;
}

int64_t variant_map_get_int64(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return variant_int64(item->value);
	}
	return -1;
}

uint64_t variant_map_get_uint64(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return variant_uint64(item->value);
	}
	return -1;
}

float variant_map_get_float(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return variant_float(item->value);
	}
	return 0.0/0.0;
}

double variant_map_get_double(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return variant_double(item->value);
	}
	return 0.0/0.0;
}

char *variant_map_get_char(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return variant_da_char(item->value);
	}
	return NULL;
}

eu_string_t *variant_map_get_string(eu_variant_map_t *map, const char *key)
{
	variant_map_item_t *item = variant_map_item_find(map, key);
	if (item) {
		return variant_da_string(item->value);
	}
	return NULL;
}


const eu_variant_t *variant_map_find(eu_variant_map_t *map, const char *key)
{
	eu_list_node_t *iter = NULL;
	list_for_each(iter, map->items)
	{
		variant_map_item_t *item = list_node_data(iter);
		if (strcmp(item->key, key) == 0) {
			return item->value;
		}
	}
	return NULL;
}

bool variant_map_delete(eu_variant_map_t *map, const char *key)
{
	eu_list_node_t *iter = NULL;
	list_for_each(iter, map->items)
	{
		variant_map_item_t *item = list_node_data(iter);
		if (strcmp(item->key, key) == 0) {
			variant_map_item_t *item = list_node_data(iter);
			variant_map_item_destroy(item);
			list_remove_node(map->items, iter);
			return true;
		}
	}

	return false;
}

eu_variant_t *variant_map_take(eu_variant_map_t *map, const char *key)
{
	eu_variant_t *var = NULL;
	eu_list_node_t *iter = NULL;
	list_for_each(iter, map->items)
	{
		variant_map_item_t *item = list_node_data(iter);
		if (strcmp(item->key, key) == 0) {
			variant_map_item_t *item = list_node_data(iter);
			var = item->value;
			item->value = NULL;
			variant_map_item_destroy(item);
			list_remove_node(map->items, iter);
			break;
		}
	}

	return var;
}

size_t variant_map_count(eu_variant_map_t *map)
{
	return list_count(map->items);
}

bool variant_map_is_empty(eu_variant_map_t *map)
{
	return (list_count(map->items) > 0) ?  false : true;
}

