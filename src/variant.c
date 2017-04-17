/*
 * variant.c
 *
 *  Created on: Apr 11, 2016
 *      Author: jnevens
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#include "eu/variant.h"

struct eu_variant_s
{
	eu_variant_type_t type;
	union{
		bool		b;
		int8_t		i8;
		uint8_t		u8;
		int16_t		i16;
		uint16_t	u16;
		int32_t		i32;
		uint32_t	u32;
		int64_t		i64;
		uint64_t	u64;
		float		f;
		double		d;
		char*		c;
		eu_string_t*	s;
		eu_variant_map_t *	m;
	} data;
};


eu_variant_t *eu_variant_create(eu_variant_type_t type)
{
	eu_variant_t *variant = calloc(1, sizeof(eu_variant_t));
	variant->type = type;
	if(type == EU_VARIANT_TYPE_STRING) {
		variant->data.s = eu_string_create();
	}
	return variant;
}

void eu_variant_destroy(eu_variant_t *variant)
{
	if (variant) {
		if (variant->type == EU_VARIANT_TYPE_CHAR) {
			free(variant->data.c);
		} else if (variant->type == EU_VARIANT_TYPE_STRING) {
			eu_string_destroy(variant->data.s);
		} else if (variant->type == EU_VARIANT_TYPE_MAP) {
			eu_variant_map_destroy(variant->data.m);
		}
		free(variant);
	}
}

eu_variant_type_t eu_variant_type(eu_variant_t *variant)
{
	return variant->type;
}

bool eu_variant_copy(eu_variant_t *dest, eu_variant_t *src)
{
	if(!dest || !src || dest->type != src->type) {
		return false;
	}

	if(src->type == EU_VARIANT_TYPE_CHAR) {
		free(dest->data.c);
		dest->data.c = strdup(src->data.c);
	} else if (src->type == EU_VARIANT_TYPE_STRING) {
		eu_string_copy(dest->data.s, src->data.s);
	} else {
		dest->data = src->data;
	}

	return true;
}

eu_variant_t *eu_variant_duplicate(eu_variant_t *variant)
{
	eu_variant_t *copy = NULL;
	if (!variant) {
		return NULL;
	}
	copy = calloc(1, sizeof(eu_variant_t));
	if (copy) {
		memcpy(copy, variant, sizeof(eu_variant_t));
		if(copy->type == EU_VARIANT_TYPE_CHAR) {
			copy->data.c = strdup(variant->data.c);
		}
	}

	return copy;
}

bool eu_variant_set_bool(eu_variant_t *variant, bool data)
{
	if (!variant || variant->type != EU_VARIANT_TYPE_BOOL) {
		return false;
	}

	variant->data.b = data;

	return true;
}

bool eu_variant_set_int8(eu_variant_t *variant, int8_t data)
{
	if (!variant || variant->type != EU_VARIANT_TYPE_INT8) {
		return false;
	}

	variant->data.i8 = data;

	return true;
}

bool eu_variant_set_uint8(eu_variant_t *variant, uint8_t data)
{
	if (!variant || variant->type != EU_VARIANT_TYPE_UINT8) {
		return false;
	}

	variant->data.u8 = data;

	return true;
}

bool eu_variant_set_int16(eu_variant_t *variant, int16_t data)
{
	if (!variant || variant->type != EU_VARIANT_TYPE_INT16) {
		return false;
	}

	variant->data.i16 = data;

	return true;
}

bool eu_variant_set_uint16(eu_variant_t *variant, uint16_t data)
{
	if (!variant || variant->type != EU_VARIANT_TYPE_UINT16) {
		return false;
	}

	variant->data.u16 = data;

	return true;
}

bool eu_variant_set_int32(eu_variant_t *variant, int32_t data)
{
	if (!variant || variant->type != EU_VARIANT_TYPE_INT32) {
		return false;
	}

	variant->data.i32 = data;

	return true;
}

bool eu_variant_set_uint32(eu_variant_t *variant, uint32_t data)
{
	if (!variant || variant->type != EU_VARIANT_TYPE_UINT32) {
		return false;
	}

	variant->data.u32 = data;

	return true;
}

bool eu_variant_set_int64(eu_variant_t *variant, int64_t data)
{
	if (!variant || variant->type != EU_VARIANT_TYPE_INT64) {
		return false;
	}

	variant->data.i64 = data;

	return true;
}

bool eu_variant_set_uint64(eu_variant_t *variant, uint64_t data)
{
	if (!variant || variant->type != EU_VARIANT_TYPE_UINT64) {
		return false;
	}

	variant->data.u64 = data;

	return true;
}

bool eu_variant_set_float(eu_variant_t *variant, float data)
{
	if (!variant || variant->type != EU_VARIANT_TYPE_FLOAT) {
		return false;
	}

	variant->data.f = data;

	return true;
}

bool eu_variant_set_double(eu_variant_t *variant, double data)
{
	if (!variant || variant->type != EU_VARIANT_TYPE_DOUBLE) {
		return false;
	}

	variant->data.d = data;

	return true;
}

bool eu_variant_set_char(eu_variant_t *variant, const char *data)
{
	if (!variant || variant->type != EU_VARIANT_TYPE_CHAR) {
		return false;
	}

	free(variant->data.c);

	variant->data.c = strdup(data);

	return true;
}

bool eu_variant_set_string(eu_variant_t *variant, const eu_string_t *data)
{
	if (!variant || variant->type != EU_VARIANT_TYPE_STRING) {
		return false;
	}

	eu_string_copy(variant->data.s, data);

	return true;
}

bool eu_variant_set_map(eu_variant_t *variant, eu_variant_map_t *map)
{
	if (!variant || variant->type != EU_VARIANT_TYPE_MAP) {
		return false;
	}

	variant->data.m = map;

	return true;
}

bool eu_variant_bool(const eu_variant_t *variant)
{
	return variant->data.b;
}

int8_t eu_variant_int8(const eu_variant_t *variant)
{
	return variant->data.i8;
}

uint8_t eu_variant_uint8(const eu_variant_t *variant)
{
	return variant->data.u8;
}

int16_t eu_variant_int16(const eu_variant_t *variant)
{
	return variant->data.i16;
}

uint16_t eu_variant_uint16(const eu_variant_t *variant)
{
	return variant->data.u16;
}

int32_t eu_variant_int32(const eu_variant_t *variant)
{
	return variant->data.i32;
}

uint32_t eu_variant_uint32(const eu_variant_t *variant)
{
	return variant->data.u32;
}

int64_t eu_variant_int64(const eu_variant_t *variant)
{
	return variant->data.i64;
}

uint64_t eu_variant_uint64(const eu_variant_t *variant)
{
	return variant->data.u64;
}

float eu_variant_float(const eu_variant_t *variant)
{
	return variant->data.f;
}

double eu_variant_double(const eu_variant_t *variant)
{
	return variant->data.d;
}

char *eu_variant_char(const eu_variant_t *variant)
{
	return strdup(variant->data.c);
}

eu_string_t *eu_variant_string(const eu_variant_t *variant)
{
	return (variant) ? eu_string_duplicate(variant->data.s) : NULL;
}

const char *eu_variant_da_char(const eu_variant_t *variant)
{
	return (variant) ? variant->data.c : NULL;
}

const eu_string_t *eu_variant_da_string(const eu_variant_t *variant)
{
	return (variant) ? variant->data.s : NULL;
}

eu_variant_map_t *eu_variant_da_map(const eu_variant_t *variant)
{
	return (variant) ? variant->data.m : NULL;
}

void eu_variant_print(eu_variant_t *variant)
{
	switch(variant->type) {
		case EU_VARIANT_TYPE_BOOL:
			printf("%s", variant->data.b ? "1" : "0");
			break;
		case EU_VARIANT_TYPE_INT8:
			printf("%d", variant->data.i8);
			break;
		case EU_VARIANT_TYPE_UINT8:
			printf("%u", variant->data.u8);
			break;
		case EU_VARIANT_TYPE_INT16:
			printf("%d", variant->data.i16);
			break;
		case EU_VARIANT_TYPE_UINT16:
			printf("%u", variant->data.u16);
			break;
		case EU_VARIANT_TYPE_INT32:
			printf("%d", variant->data.i32);
			break;
		case EU_VARIANT_TYPE_UINT32:
			printf("%u", variant->data.u32);
			break;
		case EU_VARIANT_TYPE_INT64:
			printf("%" PRId64 "", variant->data.i64);
			break;
		case EU_VARIANT_TYPE_UINT64:
			printf("%" PRIu64 "", variant->data.u64);
			break;
		case EU_VARIANT_TYPE_FLOAT:
			printf("%f", variant->data.f);
			break;
		case EU_VARIANT_TYPE_DOUBLE:
			printf("%f", variant->data.d);
			break;
		case EU_VARIANT_TYPE_CHAR:
			printf("%s", variant->data.c);
			break;
		case EU_VARIANT_TYPE_STRING:
			printf("%s", eu_string_to_da_char(variant->data.s));
			break;
		default:
			printf("Not implemented!");
			break;
	}
}
