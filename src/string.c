/*
 * string.c
 *
 *  Created on: Apr 29, 2016
 *      Author: jnevens
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eu/string.h"

const char *strnull = "NULL";

struct eu_string_s
{
	char *buf;
	size_t len;
};

eu_string_t *string_create(void)
{
	return calloc(1, sizeof(eu_string_t));
}

void string_cleanup(eu_string_t *str)
{
	if(str == NULL) {
		return;
	}

	free(str->buf);
	str->buf = NULL;
	str->len = 0;
}

void string_destroy(eu_string_t *str)
{
	string_cleanup(str);
	free(str);
}

size_t string_copy(eu_string_t *dest, const eu_string_t *src)
{
	size_t len = 0;
	string_cleanup(dest);
	if(!string_is_null(src)) {
		len = string_from_char(dest, src->buf);
	}
	return len;
}

eu_string_t *string_duplicate(const eu_string_t *src)
{
	eu_string_t *str = string_create();
	string_copy(str, src);
	return str;
}

bool string_is_null(const eu_string_t *str)
{
	if (str && str->buf == NULL) {
		return true;
	}

	return false;
}

bool string_is_empty(const eu_string_t *str)
{
	if(!str) {
		return false;
	}

	return (string_is_null(str) || strlen(str->buf) == 0) ? true : false;
}

size_t string_length(const eu_string_t *str)
{
	return (str != NULL && str->buf && *str->buf) ? strlen(str->buf) : 0;
}

size_t string_append(eu_string_t *str, const eu_string_t *data)
{
	return 0;
}

size_t string_prepend(eu_string_t *str, const eu_string_t *data)
{
	return 0;
}

size_t string_find(eu_string_t *str, const eu_string_t *data)
{
	return 0;
}

size_t string_replace(eu_string_t *str, const eu_string_t *data)
{
	return 0;
}

size_t string_to_upper(eu_string_t *str, const eu_string_t *data)
{
	return 0;
}

size_t string_to_lower(eu_string_t *str, const eu_string_t *data)
{
	return 0;
}

size_t string_trim(eu_string_t *str, const eu_string_t *data)
{
	return 0;
}

size_t string_from_bool(eu_string_t *string, const bool data)
{
	if(!string) {
		return 0;
	}

	string_cleanup(string);

	return string_from_char(string, (data) ? "1" : "0");
}

size_t string_from_char(eu_string_t *string, const char *data)
{
	if(!string || !data) {
		return 0;
	}

	string_cleanup(string);

	size_t slen = strlen(data);

	string->buf = calloc(1, slen + 1);
	if(string->buf == NULL) {
		return 0;
	}
	strcpy(string->buf, data);
	return slen;
}

size_t string_from_int8(eu_string_t *string, const int8_t data)
{
	char buf[5];
	sprintf(buf, "%d", data);

	return string_from_char(string, buf);
}

size_t string_from_uint8(eu_string_t *string, const uint8_t data)
{
	char buf[5];
	sprintf(buf, "%d", data);

	return string_from_char(string, buf);
}

size_t string_from_int16(eu_string_t *string, const int16_t data)
{
	char buf[7];
	sprintf(buf, "%d", data);

	return string_from_char(string, buf);
}

size_t string_from_uint16(eu_string_t *string, const uint16_t data)
{
	char buf[7];
	sprintf(buf, "%d", data);

	return string_from_char(string, buf);
}

size_t string_from_int32(eu_string_t *string, const int32_t data)
{
	return 0;
}

size_t string_from_int64(eu_string_t *string, const int64_t data)
{
	return 0;
}

size_t string_from_uint64(eu_string_t *string, const uint64_t data)
{
	return 0;
}

size_t string_from_uint32(eu_string_t *string, const uint32_t data)
{
	return 0;
}

size_t string_from_float(eu_string_t *string, const double data)
{
	return 0;
}

size_t string_from_double(eu_string_t *string, const double data)
{
	return 0;
}


char *string_to_char(eu_string_t *string)
{
	if(!string) {
		return NULL;
	}

	if(string_is_null(string)) {
		return strdup(strnull);
	}

	return strdup(string->buf);
}

const char *string_to_da_char(eu_string_t *string)
{
	if(!string) {
		return NULL;
	}

	if(string_is_null(string)) {
		return strnull;
	}

	return string->buf;
}
