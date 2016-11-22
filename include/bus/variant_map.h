/*
 * variant_map.h
 *
 *  Created on: Nov 9, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_BUS_VARIANT_MAP_H_
#define INCLUDE_BUS_VARIANT_MAP_H_

#include <stdbool.h>
#include <stdint.h>
#include <bus/types.h>

variant_map_t *variant_map_create(void);

bool variant_map_set_variant(variant_map_t *map, const char *key, variant_t *var);
bool variant_map_set_bool(variant_map_t *map, const char *key, bool data);
bool variant_map_set_int8(variant_map_t *map, const char *key, int8_t data);
bool variant_map_set_uint8(variant_map_t *map, const char *key, uint8_t data);
bool variant_map_set_int16(variant_map_t *map, const char *key, int16_t data);
bool variant_map_set_uint16(variant_map_t *map, const char *key, uint16_t data);
bool variant_map_set_int32(variant_map_t *map, const char *key, int32_t data);
bool variant_map_set_uint32(variant_map_t *map, const char *key, uint32_t data);
bool variant_map_set_int64(variant_map_t *map, const char *key, int64_t data);
bool variant_map_set_uint64(variant_map_t *map, const char *key, uint64_t data);
bool variant_map_set_float(variant_map_t *map, const char *key, float data);
bool variant_map_set_double(variant_map_t *map, const char *key, double data);
bool variant_map_set_char(variant_map_t *map, const char *key, const char *data);
bool variant_map_set_string(variant_map_t *map, const char *key, const string_t *data);

variant_t *variant_map_get_variant(variant_map_t *map, const char *key);
bool variant_map_get_bool(variant_map_t *map, const char *key);
int8_t variant_map_get_int8(variant_map_t *map, const char *key);
uint8_t variant_map_get_uint8(variant_map_t *map, const char *key);
int16_t variant_map_get_int16(variant_map_t *map, const char *key);
uint16_t variant_map_get_uint16(variant_map_t *map, const char *key);
int32_t variant_map_get_int32(variant_map_t *map, const char *key);
uint32_t variant_map_get_uint32(variant_map_t *map, const char *key);
int64_t variant_map_get_int64(variant_map_t *map, const char *key);
uint64_t variant_map_get_uint64(variant_map_t *map, const char *key);
float variant_map_get_float(variant_map_t *map, const char *key);
double variant_map_get_double(variant_map_t *map, const char *key);
char *variant_map_get_char(variant_map_t *map, const char *key);
string_t *variant_map_get_string(variant_map_t *map, const char *key);

const variant_t *variant_map_find(variant_map_t *map, const char *key);
bool variant_map_delete(variant_map_t *map, const char *key);
variant_t *variant_map_take(variant_map_t *map, const char *key);
size_t variant_map_count(variant_map_t *map);
bool variant_map_is_empty(variant_map_t *map);
void variant_map_destroy(variant_map_t *map);


#endif /* INCLUDE_BUS_VARIANT_MAP_H_ */
