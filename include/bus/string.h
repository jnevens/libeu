/*
 * string.h
 *
 *  Created on: Apr 29, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_BUS_STRING_H_
#define INCLUDE_BUS_STRING_H_

#include <stdbool.h>
#include <stdint.h>
#include "bus/types.h"

string_t *string_create(void);
void string_destroy(string_t *str);
void string_cleanup(string_t *str);

size_t string_copy(string_t *dest, const string_t *src);

bool string_is_null(string_t *str);
bool string_is_empty(string_t *str);
size_t string_length(string_t *str);

size_t string_from_bool(string_t *string, const bool data);
size_t string_from_char(string_t *string, const char *data);
size_t string_from_int8(string_t *string, const int8_t data);
size_t string_from_uint8(string_t *string, const uint8_t data);
size_t string_from_int16(string_t *string, const int16_t data);
size_t string_from_uint16(string_t *string, const uint16_t data);
size_t string_from_int32(string_t *string, const int32_t data);
size_t string_from_int64(string_t *string, const int64_t data);
size_t string_from_uint64(string_t *string, const uint64_t data);
size_t string_from_uint32(string_t *string, const uint32_t data);
size_t string_from_float(string_t *string, const double data);
size_t string_from_double(string_t *string, const double data);

char *string_to_char(string_t *string);
const char *string_to_da_char(string_t *string);

#endif /* INCLUDE_BUS_STRING_H_ */
