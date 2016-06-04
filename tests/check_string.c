#include <check.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

#include <bus/object.h>
#include <bus/parameter.h>
#include <bus/variant.h>
#include <bus/list.h>
#include <bus/string.h>
#include <bus/log.h>
#include <bus/event.h>
#include <bus/timer.h>

START_TEST(test_string_create_destroy)
{
	string_t *str = string_create();
	string_from_char(str, "test");
	ck_assert_int_eq(string_length(str), 4);
	ck_assert_str_eq(string_to_da_char(str), "test");
	string_destroy(str);
}END_TEST

START_TEST(test_string_cleanup)
{
	string_t *str = string_create();
	string_from_char(str, "test");
	string_cleanup(str);
	ck_assert_int_eq(string_length(str), 0);
	ck_assert_int_eq(string_is_empty(str), 1);
	ck_assert_int_eq(string_is_null(str), 1);
	ck_assert_str_eq(string_to_da_char(str), "NULL");
	string_destroy(str);
}END_TEST

START_TEST(test_string_copy)
{
	string_t *str = string_create();
	string_t *str2 = string_create();
	string_from_char(str, "test");
	ck_assert_int_eq(string_copy(str2, str), 4);
	ck_assert_str_eq(string_to_da_char(str), "test");
	ck_assert_str_eq(string_to_da_char(str2), "test");
	string_destroy(str);
	string_destroy(str2);
}END_TEST

START_TEST(test_null_string_copy)
{
	string_t *str = string_create();
	string_t *str2 = string_create();
	ck_assert_int_eq(string_copy(str2, str), 0);
	ck_assert_int_eq(string_is_null(str), 1);
	ck_assert_int_eq(string_is_null(str2), 1);
	string_destroy(str);
	string_destroy(str2);
}END_TEST

int main(void)
{
	int number_failed;
	Suite *s = suite_create("bus_string");
	TCase *tc_core = tcase_create("Core");
	tcase_set_timeout(tc_core, 1.0);

	tcase_add_test(tc_core, test_string_create_destroy);
	tcase_add_test(tc_core, test_string_cleanup);
	tcase_add_test(tc_core, test_string_copy);
	tcase_add_test(tc_core, test_null_string_copy);

	suite_add_tcase(s, tc_core);
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

