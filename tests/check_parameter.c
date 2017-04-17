#include <check.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

#include <eu/parameter.h>

START_TEST(test_parameter_create_destroy)
{
	eu_parameter_t *param = eu_parameter_create(NULL, "https", EU_PARAMETER_TYPE_BOOL);
	ck_assert_ptr_ne(param, NULL);
	ck_assert_str_eq(eu_parameter_name(param), "https");
	eu_parameter_destroy(param);
}END_TEST

START_TEST(test_parameter_name)
{
	eu_parameter_t *param = eu_parameter_create(NULL, "https", EU_PARAMETER_TYPE_BOOL);
	ck_assert_ptr_ne(param, NULL);
	ck_assert_str_eq(eu_parameter_name(param), "https");
	eu_parameter_destroy(param);

	ck_assert_ptr_eq(eu_parameter_name(NULL), NULL);
}END_TEST

int main(void)
{
	int number_failed;
	Suite *s = suite_create("eu_parameter");
	TCase *tc_core = tcase_create("Core");
	tcase_set_timeout(tc_core, 1.0);

	tcase_add_test(tc_core, test_parameter_create_destroy);
	tcase_add_test(tc_core, test_parameter_name);

	suite_add_tcase(s, tc_core);
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

