#include <check.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

#include <eu/object.h>
#include <eu/parameter.h>
#include <eu/variant.h>
#include <eu/list.h>
#include <eu/string.h>
#include <eu/log.h>
#include <eu/event.h>

START_TEST(test_string_create_destroy)
{
	eu_string_t *str = eu_string_create();
	eu_string_from_char(str, "test");
	ck_assert_int_eq(eu_string_length(str), 4);
	ck_assert_str_eq(eu_string_to_da_char(str), "test");
	eu_string_destroy(str);
}END_TEST

START_TEST(test_string_cleanup)
{
	eu_string_t *str = eu_string_create();
	eu_string_from_char(str, "test");
	eu_string_cleanup(str);
	ck_assert_int_eq(eu_string_length(str), 0);
	ck_assert_int_eq(eu_string_is_empty(str), 1);
	ck_assert_int_eq(eu_string_is_null(str), 1);
	ck_assert_str_eq(eu_string_to_da_char(str), "NULL");
	eu_string_destroy(str);
}END_TEST

START_TEST(test_string_copy)
{
	eu_string_t *str = eu_string_create();
	eu_string_t *str2 = eu_string_create();
	eu_string_from_char(str, "test");
	ck_assert_int_eq(eu_string_copy(str2, str), 4);
	ck_assert_str_eq(eu_string_to_da_char(str), "test");
	ck_assert_str_eq(eu_string_to_da_char(str2), "test");
	eu_string_destroy(str);
	eu_string_destroy(str2);
}END_TEST

START_TEST(test_null_string_copy)
{
	eu_string_t *str = eu_string_create();
	eu_string_t *str2 = eu_string_create();
	ck_assert_int_eq(eu_string_copy(str2, str), 0);
	ck_assert_int_eq(eu_string_is_null(str), 1);
	ck_assert_int_eq(eu_string_is_null(str2), 1);
	eu_string_destroy(str);
	eu_string_destroy(str2);
}END_TEST

int main(void)
{
	int number_failed;
	Suite *s = suite_create("eu_string");
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

