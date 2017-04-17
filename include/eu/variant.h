/*
 * eu_variant.h
 *
 *  Created on: Apr 11, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_VARIANT_H_
#define INCLUDE_VARIANT_H_

#include <stdint.h>
#include <stdbool.h>
#include "eu/types.h"
#include "eu/string.h"

typedef enum
{
	EU_VARIANT_TYPE_INVALID = 0, /* invalid! */
	EU_VARIANT_TYPE_BOOL,
	EU_VARIANT_TYPE_INT8,
	EU_VARIANT_TYPE_UINT8,
	EU_VARIANT_TYPE_INT16,
	EU_VARIANT_TYPE_UINT16,
	EU_VARIANT_TYPE_INT32,
	EU_VARIANT_TYPE_UINT32,
	EU_VARIANT_TYPE_INT64,
	EU_VARIANT_TYPE_UINT64,
	EU_VARIANT_TYPE_FLOAT,
	EU_VARIANT_TYPE_DOUBLE,
	EU_VARIANT_TYPE_CHAR,
	EU_VARIANT_TYPE_STRING,
	EU_VARIANT_TYPE_MAP,
} eu_variant_type_t;


// create
eu_variant_t *eu_variant_create(eu_variant_type_t type);

// destroy
void eu_variant_destroy(eu_variant_t *variant);
//#define auto_variant_t __attribute__((__cleanup__(variant_destroy))) variant_t
//// #define autostring __attribute((cleanup(fut_cleanup_string))) char*
//// void fut_cleanup_string(char** c);

// info
eu_variant_type_t eu_variant_type(eu_variant_t *variant);
bool eu_variant_copy(eu_variant_t *dest, eu_variant_t *src);
eu_variant_t *eu_variant_duplicate(eu_variant_t *variant);

// set
bool eu_variant_set_bool(eu_variant_t *variant, bool data);
bool eu_variant_set_int8(eu_variant_t *variant, int8_t data);
bool eu_variant_set_uint8(eu_variant_t *variant, uint8_t data);
bool eu_variant_set_int16(eu_variant_t *variant, int16_t data);
bool eu_variant_set_uint16(eu_variant_t *variant, uint16_t data);
bool eu_variant_set_int32(eu_variant_t *variant, int32_t data);
bool eu_variant_set_uint32(eu_variant_t *variant, uint32_t data);
bool eu_variant_set_int64(eu_variant_t *variant, int64_t data);
bool eu_variant_set_uint64(eu_variant_t *variant, uint64_t data);
bool eu_variant_set_float(eu_variant_t *variant, float data);
bool eu_variant_set_double(eu_variant_t *variant, double data);
bool eu_variant_set_char(eu_variant_t *variant, const char *data);
bool eu_variant_set_string(eu_variant_t *variant, const eu_string_t *data);
bool eu_variant_set_map(eu_variant_t *variant, eu_variant_map_t *map);
// todo: arrays,

// get
bool eu_variant_bool(const eu_variant_t *variant);
int8_t eu_variant_int8(const eu_variant_t *variant);
uint8_t eu_variant_uint8(const eu_variant_t *variant);
int16_t eu_variant_int16(const eu_variant_t *variant);
uint16_t eu_variant_uint16(const eu_variant_t *variant);
int32_t eu_variant_int32(const eu_variant_t *variant);
uint32_t eu_variant_uint32(const eu_variant_t *variant);
int64_t eu_variant_int64(const eu_variant_t *variant);
uint64_t eu_variant_uint64(const eu_variant_t *variant);
float eu_variant_float(const eu_variant_t *variant);
double eu_variant_double(const eu_variant_t *variant);
char *eu_variant_char(const eu_variant_t *variant);
eu_string_t *eu_variant_string(const eu_variant_t *variant);

const char *eu_variant_da_char(const eu_variant_t *variant);
const eu_string_t *eu_variant_da_string(const eu_variant_t *variant);
eu_variant_map_t *eu_variant_da_map(const eu_variant_t *variant);

//debug
void eu_variant_print(eu_variant_t *variant);

#endif /* INCLUDE_VARIANT_H_ */
