#include <check.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

#include <eu/object.h>
#include <eu/parameter.h>

START_TEST(test_object_create_destroy)
{
	object_t *parent = object_create(NULL, "com");
	ck_assert_ptr_ne(parent, NULL);
	ck_assert_str_eq(object_name(parent), "com");
	object_destroy(parent);
}END_TEST

START_TEST(test_object_add_child)
{
	object_t *parent = object_create(NULL, "com");
	object_t *child = object_create(parent, "linux");

	ck_assert_ptr_eq(object_get_first_child(parent), child);
	ck_assert_ptr_eq(object_get_last_child(parent), child);
	ck_assert_ptr_eq(object_get_child(parent, "linux"), child);
	ck_assert_ptr_eq(object_get_parent(child), parent);

	object_destroy(parent);
}END_TEST

START_TEST(test_object_get_nonexisting_child)
{
	object_t *parent = object_create(NULL, "com");

	ck_assert_ptr_eq(object_get_first_child(parent), NULL);
	ck_assert_ptr_eq(object_get_last_child(parent), NULL);
	ck_assert_ptr_eq(object_get_child(parent, "linux"), NULL);

	object_destroy(parent);
}END_TEST

START_TEST(test_object_big_test)
{
	object_t *parent = object_create(NULL, "com");
	object_t *child1 = object_create(parent, "dell");
	object_t *child2 = object_create(parent, "hp");
	object_t *child3 = object_create(parent, "msi");
	object_t *child4 = object_create(parent, "lenovo");
	object_t *child2_1 = object_create(child2, "www");

	parameter_create(child1, "https", PARAMETER_TYPE_BOOL);
	object_parameter_set_bool(child1, "https", false);
	parameter_create(child2, "https", PARAMETER_TYPE_BOOL);
	object_parameter_set_bool(child2, "https", true);
	parameter_create(child3, "https", PARAMETER_TYPE_BOOL);
	object_parameter_set_bool(child3, "https", false);
	parameter_create(child4, "https", PARAMETER_TYPE_BOOL);
	object_parameter_set_bool(child4, "https", true);
	parameter_create(child2_1, "isWebsite", PARAMETER_TYPE_BOOL);
	object_parameter_set_bool(child2_1, "isWebsite", true);

	printf("parent: %s has %zu children\n", object_name(parent), object_children_count(parent));
	printf("parent: %s has %zu children\n", object_name(parent), object_children_count(parent));

	object_t *it = object_get_first_child(parent);
	printf("parent %s, list children asc:\n", object_name(parent));
	do {
		printf("child %s\n", object_name(it));
	} while ((it = object_get_next_child(parent)) != NULL);

	it = object_get_last_child(parent);
	printf("parent %s, list children desc:\n", object_name(parent));
	do {
		printf("child %s\n", object_name(it));
	} while ((it = object_get_previous_child(parent)) != NULL);

	printf("print tree:\n");
	object_print(parent);
	object_destroy(parent);
} END_TEST

START_TEST(test_object_parameter_count)
{
	object_t *obj = object_create(NULL, "com");
	parameter_create(obj, "https", PARAMETER_TYPE_BOOL);
	parameter_create(obj, "http", PARAMETER_TYPE_BOOL);
	parameter_create(obj, "auth", PARAMETER_TYPE_BOOL);
	ck_assert_int_eq(object_get_parameter_count(obj), 3);
	object_destroy(obj);
} END_TEST

START_TEST(test_object_has_parameter)
{
	object_t *obj = object_create(NULL, "com");
	parameter_create(obj, "https", PARAMETER_TYPE_BOOL);
	parameter_create(obj, "http", PARAMETER_TYPE_BOOL);
	parameter_create(obj, "auth", PARAMETER_TYPE_BOOL);
	ck_assert_int_eq(object_has_parameter(obj, "https"), 1);
	ck_assert_int_eq(object_has_parameter(obj, "http"), 1);
	ck_assert_int_eq(object_has_parameter(obj, "auth"), 1);
	ck_assert_int_eq(object_has_parameter(obj, "foobar"), 0);
	object_destroy(obj);
} END_TEST

START_TEST(test_object_has_parameters)
{
	object_t *obj = object_create(NULL, "com");
	parameter_create(obj, "https", PARAMETER_TYPE_BOOL);
	parameter_create(obj, "http", PARAMETER_TYPE_BOOL);
	parameter_create(obj, "auth", PARAMETER_TYPE_BOOL);
	ck_assert_int_eq(object_has_parameters(obj), 1);
	object_destroy(obj);
} END_TEST

START_TEST(test_object_parameter_count_invalid_object)
{
	ck_assert_int_eq(object_get_parameter_count(NULL), 0);
} END_TEST

int main(void)
{
	int number_failed;
	Suite *s = suite_create("eu_objcect");
	TCase *tc_core = tcase_create("Core");
	tcase_set_timeout(tc_core, 1.0);

	tcase_add_test(tc_core, test_object_create_destroy);
	tcase_add_test(tc_core, test_object_add_child);
	tcase_add_test(tc_core, test_object_get_nonexisting_child);
	tcase_add_test(tc_core, test_object_big_test);
	tcase_add_test(tc_core, test_object_parameter_count);
	tcase_add_test(tc_core, test_object_has_parameter);
	tcase_add_test(tc_core, test_object_has_parameters);
	tcase_add_test(tc_core, test_object_parameter_count_invalid_object);

	suite_add_tcase(s, tc_core);
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

