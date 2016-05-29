/*
 * string.c
 *
 *  Created on: Apr 29, 2016
 *      Author: jnevens
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bus/string.h"

const char *strnull = "NULL";

struct string_s
{
	char *buf;
	size_t len;
};

string_t *string_create(void)
{
	return calloc(1, sizeof(string_t));
}

void string_cleanup(string_t *str)
{
	if(str == NULL) {
		return;
	}

	free(str->buf);
	str->buf = NULL;
	str->len = 0;
}

void string_destroy(string_t *str)
{
	string_cleanup(str);
	free(str);
}

size_t string_copy(string_t *dest, const string_t *src)
{
	// TODO
	return 0;
}

bool string_is_null(string_t *str)
{
	if (str && str->buf == NULL) {
		return true;
	}

	return false;
}

bool string_is_empty(string_t *str)
{
	if(!str) {
		return false;
	}

	return (string_is_null(str) || strlen(str->buf) == 0) ? true : false;
}

size_t string_length(string_t *str)
{
	return (str != NULL) ? strlen(str->buf) : 0;
}

size_t string_from_bool(string_t *string, const bool data)
{
	if(!string) {
		return 0;
	}

	string_cleanup(string);

	return string_from_char(string, (data) ? "1" : "0");
}

size_t string_from_char(string_t *string, const char *data)
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

size_t string_from_int8(string_t *string, const int8_t data)
{
	char buf[5];
	sprintf(buf, "%d", data);

	return string_from_char(string, buf);
}

size_t string_from_uint8(string_t *string, const uint8_t data)
{
	char buf[5];
	sprintf(buf, "%d", data);

	return string_from_char(string, buf);
}

size_t string_from_int16(string_t *string, const int16_t data)
{
	char buf[7];
	sprintf(buf, "%d", data);

	return string_from_char(string, buf);
}

size_t string_from_uint16(string_t *string, const uint16_t data)
{
	char buf[7];
	sprintf(buf, "%d", data);

	return string_from_char(string, buf);
}

size_t string_from_int32(string_t *string, const int32_t data)
{
	return 0;
}

size_t string_from_int64(string_t *string, const int64_t data)
{
	return 0;
}

size_t string_from_uint64(string_t *string, const uint64_t data)
{
	return 0;
}

size_t string_from_uint32(string_t *string, const uint32_t data)
{
	return 0;
}

size_t string_from_float(string_t *string, const double data)
{
	return 0;
}

size_t string_from_double(string_t *string, const double data)
{
	return 0;
}


char *string_to_char(string_t *string)
{
	if(!string) {
		return NULL;
	}

	if(string_is_null(string)) {
		return strdup(strnull);
	}

	return strdup(string->buf);
}

const char *string_to_da_char(string_t *string)
{
	if(!string) {
		return NULL;
	}

	if(string_is_null(string)) {
		return strnull;
	}

	return string->buf;
}
