#include <check.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

#include <eu/variant.h>

START_TEST(test_variant_create_destroy)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_BOOL);
	ck_assert_ptr_ne(var, NULL);
	eu_variant_destroy(var);
}END_TEST

START_TEST(test_variant_type)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_BOOL);
	ck_assert_int_eq(eu_variant_type(var), EU_VARIANT_TYPE_BOOL);
	eu_variant_destroy(var);
}END_TEST

START_TEST(test_variant_copy_same_type)
{
	eu_variant_t *src = eu_variant_create(EU_VARIANT_TYPE_BOOL);
	eu_variant_set_bool(src, true);
	eu_variant_t *dst = eu_variant_create(EU_VARIANT_TYPE_BOOL);
	eu_variant_set_bool(dst, false);

	ck_assert_int_eq(eu_variant_copy(dst, src), true);
	ck_assert_int_eq(eu_variant_bool(dst), true);

	eu_variant_destroy(src);
	eu_variant_destroy(dst);
}END_TEST

START_TEST(test_variant_copy_string)
{
	eu_variant_t *src = eu_variant_create(EU_VARIANT_TYPE_CHAR);
	eu_variant_set_char(src, "foobar");
	eu_variant_t *dst = eu_variant_create(EU_VARIANT_TYPE_CHAR);
	eu_variant_set_char(dst, "deadbeaf");

	ck_assert_int_eq(eu_variant_copy(dst, src), true);
	ck_assert_str_eq(eu_variant_da_char(src), "foobar");
	ck_assert_str_eq(eu_variant_da_char(dst), "foobar");

	eu_variant_destroy(src);
	eu_variant_destroy(dst);
}END_TEST

START_TEST(test_variant_copy_different_type)
{
	eu_variant_t *src = eu_variant_create(EU_VARIANT_TYPE_BOOL);
	eu_variant_set_bool(src, true);
	eu_variant_t *dst = eu_variant_create(EU_VARIANT_TYPE_CHAR);
	eu_variant_set_char(dst, "lol");

	ck_assert_int_eq(eu_variant_copy(dst, src), false);

	eu_variant_destroy(src);
	eu_variant_destroy(dst);
}END_TEST

START_TEST(test_variant_copy_invalid)
{
	eu_variant_t *src = eu_variant_create(EU_VARIANT_TYPE_BOOL);
	eu_variant_t *dst = eu_variant_create(EU_VARIANT_TYPE_CHAR);
	ck_assert_int_eq(eu_variant_copy(NULL, src), false);
	ck_assert_int_eq(eu_variant_copy(dst, NULL), false);
	eu_variant_destroy(src);
	eu_variant_destroy(dst);
}END_TEST

START_TEST(test_variant_duplicate_bool)
{
	eu_variant_t *src = eu_variant_create(EU_VARIANT_TYPE_BOOL);
	eu_variant_set_bool(src, true);
	eu_variant_t *dst = NULL;

	dst = eu_variant_duplicate(src);
	ck_assert_ptr_ne(dst, NULL);
	ck_assert_int_eq(eu_variant_type(dst), EU_VARIANT_TYPE_BOOL);
	ck_assert_int_eq(eu_variant_bool(dst), true);

	eu_variant_destroy(src);
	eu_variant_destroy(dst);
}END_TEST

START_TEST(test_variant_duplicate_char)
{
	eu_variant_t *src = eu_variant_create(EU_VARIANT_TYPE_CHAR);
	eu_variant_set_char(src, "foobar");
	eu_variant_t *dst = NULL;

	dst = eu_variant_duplicate(src);
	ck_assert_ptr_ne(dst, NULL);
	ck_assert_int_eq(eu_variant_type(dst), EU_VARIANT_TYPE_CHAR);
	ck_assert_str_eq(eu_variant_da_char(dst), "foobar");

	eu_variant_destroy(src);
	eu_variant_destroy(dst);
}END_TEST

START_TEST(test_variant_duplicate_invalid)
{
	ck_assert_ptr_eq(eu_variant_duplicate(NULL), NULL);
}END_TEST

START_TEST(test_variant_bool)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_BOOL);
	ck_assert_int_eq(eu_variant_type(var), EU_VARIANT_TYPE_BOOL);
	ck_assert_ptr_ne(var, NULL);
	ck_assert_int_eq(eu_variant_set_bool(var, true), true);
	ck_assert_int_eq(eu_variant_bool(var), true);
	ck_assert_int_eq(eu_variant_set_bool(var, false), true);
	ck_assert_int_eq(eu_variant_bool(var), false);
	eu_variant_print(var);
	eu_variant_destroy(var);
}END_TEST

START_TEST(test_variant_int8)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_INT8);
	ck_assert_int_eq(eu_variant_type(var), EU_VARIANT_TYPE_INT8);
	ck_assert_ptr_ne(var, NULL);
	ck_assert_int_eq(eu_variant_set_int8(var, -100 +1024), true);
	ck_assert_int_eq(eu_variant_int8(var), -100);
	eu_variant_print(var);
	eu_variant_destroy(var);
}END_TEST

START_TEST(test_variant_uint8)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_UINT8);
	ck_assert_int_eq(eu_variant_type(var), EU_VARIANT_TYPE_UINT8);
	ck_assert_ptr_ne(var, NULL);
	ck_assert_int_eq(eu_variant_set_uint8(var, 230 +1024), true);
	ck_assert_int_eq(eu_variant_uint8(var), 230);
	eu_variant_print(var);
	eu_variant_destroy(var);
}END_TEST

START_TEST(test_variant_int16)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_INT16);
	ck_assert_int_eq(eu_variant_type(var), EU_VARIANT_TYPE_INT16);
	ck_assert_ptr_ne(var, NULL);
	ck_assert_int_eq(eu_variant_set_int16(var, -28000), true);
	ck_assert_int_eq(eu_variant_int16(var), -28000);
	eu_variant_print(var);
	eu_variant_destroy(var);
}END_TEST

START_TEST(test_variant_uint16)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_UINT16);
	ck_assert_int_eq(eu_variant_type(var), EU_VARIANT_TYPE_UINT16);
	ck_assert_ptr_ne(var, NULL);
	ck_assert_int_eq(eu_variant_set_uint16(var, 45000), true);
	ck_assert_int_eq(eu_variant_uint16(var), 45000);
	eu_variant_print(var);
	eu_variant_destroy(var);
}END_TEST

START_TEST(test_variant_int32)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_INT32);
	ck_assert_int_eq(eu_variant_type(var), EU_VARIANT_TYPE_INT32);
	ck_assert_ptr_ne(var, NULL);
	ck_assert_int_eq(eu_variant_set_int32(var, -28000), true);
	ck_assert_int_eq(eu_variant_int32(var), -28000);
	eu_variant_print(var);
	eu_variant_destroy(var);
}END_TEST

START_TEST(test_variant_uint32)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_UINT32);
	ck_assert_int_eq(eu_variant_type(var), EU_VARIANT_TYPE_UINT32);
	ck_assert_ptr_ne(var, NULL);
	ck_assert_int_eq(eu_variant_set_uint32(var, 45000), true);
	ck_assert_int_eq(eu_variant_uint32(var), 45000);
	eu_variant_print(var);
	eu_variant_destroy(var);
}END_TEST

START_TEST(test_variant_int64)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_INT64);
	ck_assert_int_eq(eu_variant_type(var), EU_VARIANT_TYPE_INT64);
	ck_assert_ptr_ne(var, NULL);
	ck_assert_int_eq(eu_variant_set_int64(var, -28000), true);
	ck_assert_int_eq(eu_variant_int64(var), -28000);
	eu_variant_print(var);
	eu_variant_destroy(var);
}END_TEST

START_TEST(test_variant_uint64)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_UINT64);
	ck_assert_int_eq(eu_variant_type(var), EU_VARIANT_TYPE_UINT64);
	ck_assert_ptr_ne(var, NULL);
	ck_assert_int_eq(eu_variant_set_uint64(var, 45000), true);
	ck_assert_int_eq(eu_variant_uint64(var), 45000);
	eu_variant_print(var);
	eu_variant_destroy(var);
}END_TEST

START_TEST(test_variant_float)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_FLOAT);
	ck_assert_int_eq(eu_variant_type(var), EU_VARIANT_TYPE_FLOAT);
	ck_assert_ptr_ne(var, NULL);
	ck_assert_int_eq(eu_variant_set_float(var, 123.456), true);
	ck_assert_int_eq(eu_variant_float(var), 123.45);
	eu_variant_print(var);
	eu_variant_destroy(var);
}END_TEST

START_TEST(test_variant_double)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_DOUBLE);
	ck_assert_int_eq(eu_variant_type(var), EU_VARIANT_TYPE_DOUBLE);
	ck_assert_ptr_ne(var, NULL);
	ck_assert_int_eq(eu_variant_set_double(var, 12345789.12345678), true);
	ck_assert_int_eq(eu_variant_double(var), 12345789.123456785);
	eu_variant_print(var);
	eu_variant_destroy(var);
}END_TEST

START_TEST(test_variant_char)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_CHAR);
	ck_assert_int_eq(eu_variant_type(var), EU_VARIANT_TYPE_CHAR);
	ck_assert_ptr_ne(var, NULL);
	ck_assert_int_eq(eu_variant_set_char(var, "foobar"), true);
	ck_assert_str_eq(eu_variant_da_char(var), "foobar");
	char *data = eu_variant_char(var);
	ck_assert_str_eq(data, "foobar");
	free(data);
	eu_variant_print(var);
	eu_variant_destroy(var);
}END_TEST

START_TEST(test_variant_string)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_STRING);
	ck_assert_int_eq(eu_variant_type(var), EU_VARIANT_TYPE_STRING);
	ck_assert_ptr_ne(var, NULL);
	eu_string_t *str = eu_string_create();
	eu_string_from_char(str, "foobar");
	ck_assert_int_eq(eu_variant_set_string(var, str), true);
	ck_assert_str_eq(eu_string_to_da_char(eu_variant_da_string(var)), "foobar");

	eu_string_t *res = eu_variant_string(var);
	ck_assert_str_eq(eu_string_to_da_char(res), "foobar");

	eu_variant_print(var);
	eu_variant_destroy(var);
	eu_string_destroy(str);
	eu_string_destroy(res);
}END_TEST

int main(void)
{
	int number_failed;
	Suite *s = suite_create("eu_variant");
	TCase *tc_core = tcase_create("Core");
	tcase_set_timeout(tc_core, 1.0);

	tcase_add_test(tc_core, test_variant_create_destroy);
	tcase_add_test(tc_core, test_variant_type);
	tcase_add_test(tc_core, test_variant_copy_same_type);
	tcase_add_test(tc_core, test_variant_copy_string);
	tcase_add_test(tc_core, test_variant_copy_different_type);
	tcase_add_test(tc_core, test_variant_copy_invalid);
	tcase_add_test(tc_core, test_variant_duplicate_bool);
	tcase_add_test(tc_core, test_variant_duplicate_char);
	tcase_add_test(tc_core, test_variant_duplicate_invalid);
	tcase_add_test(tc_core, test_variant_bool);
	tcase_add_test(tc_core, test_variant_int8);
	tcase_add_test(tc_core, test_variant_uint8);
	tcase_add_test(tc_core, test_variant_int16);
	tcase_add_test(tc_core, test_variant_uint16);
	tcase_add_test(tc_core, test_variant_int32);
	tcase_add_test(tc_core, test_variant_uint32);
	tcase_add_test(tc_core, test_variant_int64);
	tcase_add_test(tc_core, test_variant_uint64);
	tcase_add_test(tc_core, test_variant_float);
	tcase_add_test(tc_core, test_variant_double);
	tcase_add_test(tc_core, test_variant_char);
	tcase_add_test(tc_core, test_variant_string);

	suite_add_tcase(s, tc_core);
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

