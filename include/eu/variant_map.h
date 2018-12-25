/*
 * variant_map.h
 *
 *  Created on: Nov 9, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_EU_VARIANT_MAP_H_
#define INCLUDE_EU_VARIANT_MAP_H_

#include <stdbool.h>
#include <stdint.h>
#include <eu/types.h>
#include <json-c/json.h>

// loop all child's
#define eu_variant_map_for_each_pair(pair, map) \
		for(eu_variant_map_pair_t *pair = eu_variant_map_get_first(map); pair; pair = eu_variant_map_get_next(pair))

#define eu_variant_map_for_each_pair_reverse(pair, map) \
		for(eu_variant_map_pair_t *pair = eu_variant_map_get_last(map); pair; pair = eu_variant_map_get_prev(pair))

/* variant map pair */
eu_variant_map_pair_t *eu_variant_map_pair_create(const char *key, eu_variant_t *var);
void eu_variant_map_pair_destroy(void *arg);
const char *eu_variant_map_pair_get_key(eu_variant_map_pair_t *map_item);
eu_variant_t *eu_variant_map_pair_get_val(eu_variant_map_pair_t *map_item);
eu_variant_map_t *eu_variant_map_pair_get_map(eu_variant_map_pair_t *map_item);

/* variant map */
eu_variant_map_t *eu_variant_map_create(void);
void eu_variant_map_destroy(eu_variant_map_t *map);

eu_variant_t *eu_variant_map_find(eu_variant_map_t *map, const char *key);
bool eu_variant_map_has(eu_variant_map_t *map, const char *key);
eu_variant_map_pair_t *eu_variant_map_find_pair(eu_variant_map_t *map, const char *key);

eu_variant_t *eu_variant_map_take(eu_variant_map_t *map, const char *key);
size_t eu_variant_map_count(eu_variant_map_t *map);
bool eu_variant_map_is_empty(eu_variant_map_t *map);

void eu_variant_map_add_pair(eu_variant_map_t *map, eu_variant_map_pair_t *pair);
bool eu_variant_map_delete(eu_variant_map_t *map, const char *key);


eu_variant_map_pair_t *eu_variant_map_get_first(eu_variant_map_t *map);
eu_variant_map_pair_t *eu_variant_map_get_last(eu_variant_map_t *map);
eu_variant_map_pair_t *eu_variant_map_get_next(eu_variant_map_pair_t *map_entry);
eu_variant_map_pair_t *eu_variant_map_get_prev(eu_variant_map_pair_t *map_etry);

bool eu_variant_map_set_variant(eu_variant_map_t *map, const char *key, eu_variant_t *var);
bool eu_variant_map_set_bool(eu_variant_map_t *map, const char *key, bool data);
bool eu_variant_map_set_int8(eu_variant_map_t *map, const char *key, int8_t data);
bool eu_variant_map_set_uint8(eu_variant_map_t *map, const char *key, uint8_t data);
bool eu_variant_map_set_int16(eu_variant_map_t *map, const char *key, int16_t data);
bool eu_variant_map_set_uint16(eu_variant_map_t *map, const char *key, uint16_t data);
bool eu_variant_map_set_int32(eu_variant_map_t *map, const char *key, int32_t data);
bool eu_variant_map_set_uint32(eu_variant_map_t *map, const char *key, uint32_t data);
bool eu_variant_map_set_int64(eu_variant_map_t *map, const char *key, int64_t data);
bool eu_variant_map_set_uint64(eu_variant_map_t *map, const char *key, uint64_t data);
bool eu_variant_map_set_float(eu_variant_map_t *map, const char *key, float data);
bool eu_variant_map_set_double(eu_variant_map_t *map, const char *key, double data);
bool eu_variant_map_set_char(eu_variant_map_t *map, const char *key, const char *data);
bool eu_variant_map_set_string(eu_variant_map_t *map, const char *key, const eu_string_t *data);

eu_variant_t *eu_variant_map_get_variant(eu_variant_map_t *map, const char *key);
bool eu_variant_map_get_bool(eu_variant_map_t *map, const char *key);
int8_t eu_variant_map_get_int8(eu_variant_map_t *map, const char *key);
uint8_t eu_variant_map_get_uint8(eu_variant_map_t *map, const char *key);
int16_t eu_variant_map_get_int16(eu_variant_map_t *map, const char *key);
uint16_t eu_variant_map_get_uint16(eu_variant_map_t *map, const char *key);
int32_t eu_variant_map_get_int32(eu_variant_map_t *map, const char *key);
uint32_t eu_variant_map_get_uint32(eu_variant_map_t *map, const char *key);
int64_t eu_variant_map_get_int64(eu_variant_map_t *map, const char *key);
uint64_t eu_variant_map_get_uint64(eu_variant_map_t *map, const char *key);
float eu_variant_map_get_float(eu_variant_map_t *map, const char *key);
double eu_variant_map_get_double(eu_variant_map_t *map, const char *key);
char *eu_variant_map_get_char(eu_variant_map_t *map, const char *key);
eu_string_t *eu_variant_map_get_string(eu_variant_map_t *map, const char *key);

json_object *eu_variant_map_serialize(const eu_variant_map_t *map);
eu_variant_map_t *eu_variant_map_deserialize(json_object *obj);

#endif /* INCLUDE_EU_VARIANT_MAP_H_ */
