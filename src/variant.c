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

#include "bus/variant.h"

struct variant_s
{
	variant_type_t type;
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
	} data;
};


variant_t *variant_create(variant_type_t type)
{
	variant_t *variant = calloc(1, sizeof(variant_t));
	variant->type = type;
	return variant;
}

void variant_destroy(variant_t *variant)
{
	if (variant) {
		if (variant->type == VARIANT_TYPE_CHAR) {
			free(variant->data.c);
		}
		free(variant);
	}
}

variant_type_t variant_type(variant_t *variant)
{
	return variant->type;
}

variant_t *variant_copy(variant_t *variant)
{
	if (!variant) {
		return NULL;
	}
	variant_t *copy = calloc(1, sizeof(variant_t));
	if (!copy) {
		return NULL;
	}

	return memcpy(copy, variant, sizeof(variant_t));
}

bool variant_set_bool(variant_t *variant, bool data)
{
	if (!variant || variant->type != VARIANT_TYPE_BOOL) {
		return false;
	}

	variant->data.b = data;

	return true;
}

bool variant_set_int8(variant_t *variant, int8_t data)
{
	if (!variant || variant->type != VARIANT_TYPE_INT8) {
		return false;
	}

	variant->data.i8 = data;

	return true;
}

bool variant_set_uint8(variant_t *variant, uint8_t data)
{
	if (!variant || variant->type != VARIANT_TYPE_UINT8) {
		return false;
	}

	variant->data.u8 = data;

	return true;
}

bool variant_set_int16(variant_t *variant, int16_t data)
{
	if (!variant || variant->type != VARIANT_TYPE_INT16) {
		return false;
	}

	variant->data.i16 = data;

	return true;
}

bool variant_set_uint16(variant_t *variant, uint16_t data)
{
	if (!variant || variant->type != VARIANT_TYPE_UINT16) {
		return false;
	}

	variant->data.u16 = data;

	return true;
}

bool variant_set_int32(variant_t *variant, int32_t data)
{
	if (!variant || variant->type != VARIANT_TYPE_INT32) {
		return false;
	}

	variant->data.i32 = data;

	return true;
}

bool variant_set_uint32(variant_t *variant, uint32_t data)
{
	if (!variant || variant->type != VARIANT_TYPE_UINT32) {
		return false;
	}

	variant->data.u32 = data;

	return true;
}

bool variant_set_int64(variant_t *variant, int64_t data)
{
	if (!variant || variant->type != VARIANT_TYPE_INT64) {
		return false;
	}

	variant->data.i64 = data;

	return true;
}

bool variant_set_uint64(variant_t *variant, uint64_t data)
{
	if (!variant || variant->type != VARIANT_TYPE_UINT64) {
		return false;
	}

	variant->data.u64 = data;

	return true;
}

bool variant_set_float(variant_t *variant, float data)
{
	if (!variant || variant->type != VARIANT_TYPE_FLOAT) {
		return false;
	}

	variant->data.f = data;

	return true;
}

bool variant_set_double(variant_t *variant, double data)
{
	if (!variant || variant->type != VARIANT_TYPE_DOUBLE) {
		return false;
	}

	variant->data.d = data;

	return true;
}

bool variant_set_char(variant_t *variant, const char *data)
{
	if (!variant || variant->type != VARIANT_TYPE_CHAR) {
		return false;
	}

	free(variant->data.c);

	variant->data.c = strdup(data);

	return true;
}

bool variant_bool(variant_t *variant)
{
	return variant->data.b;
}

int8_t variant_int8(variant_t *variant)
{
	return variant->data.i8;
}

uint8_t variant_uint8(variant_t *variant)
{
	return variant->data.u8;
}

int16_t variant_int16(variant_t *variant)
{
	return variant->data.i16;
}

uint16_t variant_uint16(variant_t *variant)
{
	return variant->data.u16;
}

int32_t variant_int32(variant_t *variant)
{
	return variant->data.i32;
}

uint32_t variant_uint32(variant_t *variant)
{
	return variant->data.u32;
}

int64_t variant_int64(variant_t *variant)
{
	return variant->data.i64;
}

uint64_t variant_uint64(variant_t *variant)
{
	return variant->data.u64;
}

float variant_float(variant_t *variant)
{
	return variant->data.f;
}

double variant_double(variant_t *variant)
{
	return variant->data.d;
}

char *variant_char(variant_t *variant)
{
	return strdup(variant->data.c);
}

const char *variant_da_char(variant_t *variant)
{
	return variant->data.c;
}

void variant_print(variant_t *variant)
{
	switch(variant->type) {
		case VARIANT_TYPE_BOOL:
			printf("%s", variant->data.b ? "1" : "0");
			break;
		case VARIANT_TYPE_INT8:
			printf("%d", variant->data.i8);
			break;
		case VARIANT_TYPE_UINT8:
			printf("%u", variant->data.u8);
			break;
		case VARIANT_TYPE_INT16:
			printf("%d", variant->data.i16);
			break;
		case VARIANT_TYPE_UINT16:
			printf("%u", variant->data.u16);
			break;
		case VARIANT_TYPE_INT32:
			printf("%d", variant->data.i32);
			break;
		case VARIANT_TYPE_UINT32:
			printf("%u", variant->data.u32);
			break;
		case VARIANT_TYPE_INT64:
			printf("%" PRId64 "", variant->data.i64);
			break;
		case VARIANT_TYPE_UINT64:
			printf("%" PRIu64 "", variant->data.u64);
			break;
		case VARIANT_TYPE_FLOAT:
			printf("%f", variant->data.f);
			break;
		case VARIANT_TYPE_DOUBLE:
			printf("%f", variant->data.d);
			break;
		case VARIANT_TYPE_CHAR:
			printf("%s", variant->data.c);
			break;
		default:
			printf("Not implemented!");
			break;
	}
}
