/*
 * variant.h
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
	VARIANT_TYPE_INVALID = 0, /* invalid! */
	VARIANT_TYPE_BOOL,
	VARIANT_TYPE_INT8,
	VARIANT_TYPE_UINT8,
	VARIANT_TYPE_INT16,
	VARIANT_TYPE_UINT16,
	VARIANT_TYPE_INT32,
	VARIANT_TYPE_UINT32,
	VARIANT_TYPE_INT64,
	VARIANT_TYPE_UINT64,
	VARIANT_TYPE_FLOAT,
	VARIANT_TYPE_DOUBLE,
	VARIANT_TYPE_CHAR,
	VARIANT_TYPE_STRING,
	VARIANT_TYPE_MAP,
} variant_type_t;


// create
variant_t *variant_create(variant_type_t type);

// destroy
void variant_destroy(variant_t *variant);
//#define auto_variant_t __attribute__((__cleanup__(variant_destroy))) variant_t
//// #define autostring __attribute((cleanup(fut_cleanup_string))) char*
//// void fut_cleanup_string(char** c);

// info
variant_type_t variant_type(variant_t *variant);
bool variant_copy(variant_t *dest, variant_t *src);
variant_t *variant_duplicate(variant_t *variant);

// set
bool variant_set_bool(variant_t *variant, bool data);
bool variant_set_int8(variant_t *variant, int8_t data);
bool variant_set_uint8(variant_t *variant, uint8_t data);
bool variant_set_int16(variant_t *variant, int16_t data);
bool variant_set_uint16(variant_t *variant, uint16_t data);
bool variant_set_int32(variant_t *variant, int32_t data);
bool variant_set_uint32(variant_t *variant, uint32_t data);
bool variant_set_int64(variant_t *variant, int64_t data);
bool variant_set_uint64(variant_t *variant, uint64_t data);
bool variant_set_float(variant_t *variant, float data);
bool variant_set_double(variant_t *variant, double data);
bool variant_set_char(variant_t *variant, const char *data);
bool variant_set_string(variant_t *variant, const string_t *data);
bool variant_set_map(variant_t *variant, variant_map_t *map);
// todo: arrays,

// get
bool variant_bool(const variant_t *variant);
int8_t variant_int8(const variant_t *variant);
uint8_t variant_uint8(const variant_t *variant);
int16_t variant_int16(const variant_t *variant);
uint16_t variant_uint16(const variant_t *variant);
int32_t variant_int32(const variant_t *variant);
uint32_t variant_uint32(const variant_t *variant);
int64_t variant_int64(const variant_t *variant);
uint64_t variant_uint64(const variant_t *variant);
float variant_float(const variant_t *variant);
double variant_double(const variant_t *variant);
char *variant_char(const variant_t *variant);
string_t *variant_string(const variant_t *variant);

const char *variant_da_char(const variant_t *variant);
const string_t *variant_da_string(const variant_t *variant);
variant_map_t *variant_da_map(const variant_t *variant);

//debug
void variant_print(variant_t *variant);

#endif /* INCLUDE_VARIANT_H_ */
