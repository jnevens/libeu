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
	eu_object_t *parent = eu_object_create(NULL, "com");
	ck_assert_ptr_ne(parent, NULL);
	ck_assert_str_eq(eu_object_name(parent), "com");
	eu_object_destroy(parent);
}END_TEST

START_TEST(test_object_add_child)
{
	eu_object_t *parent = eu_object_create(NULL, "com");
	eu_object_t *child = eu_object_create(parent, "linux");

	ck_assert_ptr_eq(eu_object_get_first_child(parent), child);
	ck_assert_ptr_eq(eu_object_get_last_child(parent), child);
	ck_assert_ptr_eq(eu_object_get_child(parent, "linux"), child);
	ck_assert_ptr_eq(eu_object_get_parent(child), parent);

	eu_object_destroy(parent);
}END_TEST

START_TEST(test_object_get_nonexisting_child)
{
	eu_object_t *parent = eu_object_create(NULL, "com");

	ck_assert_ptr_eq(eu_object_get_first_child(parent), NULL);
	ck_assert_ptr_eq(eu_object_get_last_child(parent), NULL);
	ck_assert_ptr_eq(eu_object_get_child(parent, "linux"), NULL);

	eu_object_destroy(parent);
}END_TEST

START_TEST(test_object_big_test)
{
	eu_object_t *parent = eu_object_create(NULL, "com");
	eu_object_t *child1 = eu_object_create(parent, "dell");
	eu_object_t *child2 = eu_object_create(parent, "hp");
	eu_object_t *child3 = eu_object_create(parent, "msi");
	eu_object_t *child4 = eu_object_create(parent, "lenovo");
	eu_object_t *child2_1 = eu_object_create(child2, "www");

	eu_parameter_create(child1, "https", EU_PARAMETER_TYPE_BOOL);
	eu_object_parameter_set_bool(child1, "https", false);
	eu_parameter_create(child2, "https", EU_PARAMETER_TYPE_BOOL);
	eu_object_parameter_set_bool(child2, "https", true);
	eu_parameter_create(child3, "https", EU_PARAMETER_TYPE_BOOL);
	eu_object_parameter_set_bool(child3, "https", false);
	eu_parameter_create(child4, "https", EU_PARAMETER_TYPE_BOOL);
	eu_object_parameter_set_bool(child4, "https", true);
	eu_parameter_create(child2_1, "isWebsite", EU_PARAMETER_TYPE_BOOL);
	eu_object_parameter_set_bool(child2_1, "isWebsite", true);

	printf("parent: %s has %zu children\n", eu_object_name(parent), eu_object_children_count(parent));
	printf("parent: %s has %zu children\n", eu_object_name(parent), eu_object_children_count(parent));

	eu_object_t *it = eu_object_get_first_child(parent);
	printf("parent %s, list children asc:\n", eu_object_name(parent));
	do {
		printf("child %s\n", eu_object_name(it));
	} while ((it = eu_object_get_next_child(parent)) != NULL);

	it = eu_object_get_last_child(parent);
	printf("parent %s, list children desc:\n", eu_object_name(parent));
	do {
		printf("child %s\n", eu_object_name(it));
	} while ((it = eu_object_get_previous_child(parent)) != NULL);

	printf("print tree:\n");
	eu_object_print(parent);
	eu_object_destroy(parent);
} END_TEST

START_TEST(test_object_parameter_count)
{
	eu_object_t *obj = eu_object_create(NULL, "com");
	eu_parameter_create(obj, "https", EU_PARAMETER_TYPE_BOOL);
	eu_parameter_create(obj, "http", EU_PARAMETER_TYPE_BOOL);
	eu_parameter_create(obj, "auth", EU_PARAMETER_TYPE_BOOL);
	ck_assert_int_eq(eu_object_get_parameter_count(obj), 3);
	eu_object_destroy(obj);
} END_TEST

START_TEST(test_object_has_parameter)
{
	eu_object_t *obj = eu_object_create(NULL, "com");
	eu_parameter_create(obj, "https", EU_PARAMETER_TYPE_BOOL);
	eu_parameter_create(obj, "http", EU_PARAMETER_TYPE_BOOL);
	eu_parameter_create(obj, "auth", EU_PARAMETER_TYPE_BOOL);
	ck_assert_int_eq(eu_object_has_parameter(obj, "https"), 1);
	ck_assert_int_eq(eu_object_has_parameter(obj, "http"), 1);
	ck_assert_int_eq(eu_object_has_parameter(obj, "auth"), 1);
	ck_assert_int_eq(eu_object_has_parameter(obj, "foobar"), 0);
	eu_object_destroy(obj);
} END_TEST

START_TEST(test_object_has_parameters)
{
	eu_object_t *obj = eu_object_create(NULL, "com");
	eu_parameter_create(obj, "https", EU_PARAMETER_TYPE_BOOL);
	eu_parameter_create(obj, "http", EU_PARAMETER_TYPE_BOOL);
	eu_parameter_create(obj, "auth", EU_PARAMETER_TYPE_BOOL);
	ck_assert_int_eq(eu_object_has_parameters(obj), 1);
	eu_object_destroy(obj);
} END_TEST

START_TEST(test_object_parameter_count_invalid_object)
{
	ck_assert_int_eq(eu_object_get_parameter_count(NULL), 0);
} END_TEST

START_TEST(test_object_create_path)
{
		eu_object_t *root = eu_object_create_path(NULL, "com.google.www");
		ck_assert_ptr_eq(eu_object_create_path(root, "com.amazon.www"), root);
		eu_object_print(root);
		ck_assert_str_eq(eu_object_name(root), "com");
		ck_assert_ptr_ne(eu_object_get_child(root, "google"), NULL);
		ck_assert_ptr_ne(eu_object_get_child(root, "amazon"), NULL);
		ck_assert_ptr_ne(
				eu_object_get_child(eu_object_get_child(root, "google"), "www"),
				NULL);
		ck_assert_ptr_ne(
				eu_object_get_child(eu_object_get_child(root, "amazon"), "www"),
				NULL);
		eu_object_destroy(root);
}END_TEST

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
	tcase_add_test(tc_core, test_object_create_path);

	suite_add_tcase(s, tc_core);
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

