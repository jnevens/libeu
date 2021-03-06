/*
 * string.h
 *
 *  Created on: Apr 29, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_EU_STRING_H_
#define INCLUDE_EU_STRING_H_

#include <stdbool.h>
#include <stdint.h>
#include "eu/types.h"

eu_string_t *eu_string_create(void);
void eu_string_destroy(eu_string_t *str);
void eu_string_cleanup(eu_string_t *str);

size_t eu_string_copy(eu_string_t *dest, const eu_string_t *src);
eu_string_t *eu_string_duplicate(const eu_string_t *src);

bool eu_string_is_null(const eu_string_t *str);
bool eu_string_is_empty(const eu_string_t *str);
size_t eu_string_length(const eu_string_t *str);

size_t eu_string_append(eu_string_t *str, const eu_string_t *data);
size_t eu_string_prepend(eu_string_t *str, const eu_string_t *data);
size_t eu_string_find(eu_string_t *str, const eu_string_t *data);
size_t eu_string_replace(eu_string_t *str, const eu_string_t *data);
size_t eu_string_to_upper(eu_string_t *str, const eu_string_t *data);
size_t eu_string_to_lower(eu_string_t *str, const eu_string_t *data);
size_t eu_string_trim(eu_string_t *str, const eu_string_t *data);

size_t eu_string_from_bool(eu_string_t *string, const bool data);
size_t eu_string_from_char(eu_string_t *string, const char *data);
size_t eu_string_from_int8(eu_string_t *string, const int8_t data);
size_t eu_string_from_uint8(eu_string_t *string, const uint8_t data);
size_t eu_string_from_int16(eu_string_t *string, const int16_t data);
size_t eu_string_from_uint16(eu_string_t *string, const uint16_t data);
size_t eu_string_from_int32(eu_string_t *string, const int32_t data);
size_t eu_string_from_int64(eu_string_t *string, const int64_t data);
size_t eu_string_from_uint64(eu_string_t *string, const uint64_t data);
size_t eu_string_from_uint32(eu_string_t *string, const uint32_t data);
size_t eu_string_from_float(eu_string_t *string, const double data);
size_t eu_string_from_double(eu_string_t *string, const double data);

char *eu_string_to_char(eu_string_t *string);
const char *eu_string_to_da_char(eu_string_t *string);

#endif /* INCLUDE_EU_STRING_H_ */
