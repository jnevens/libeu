#include <check.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

#include <bus/list.h>

START_TEST(test_list_create_destroy)
{
	const char *data1 = "foo";
	const char *data2 = "bar";
	const char *data3 = "boo";
	const char *data4 = "baa";

	list_t *list = list_create();
	ck_assert_ptr_ne(list, NULL);
	list_append(list, (void *) data2);
	list_prepend(list, (void *) data1);
	list_append(list, (void *) data3);
	list_append(list, (void *) data4);

	list_node_t *iter = NULL;
	printf("list asc:\n");
	list_for_each(iter, list)
	{
		printf("%s\n", (const char *) list_node_data(iter));
	}

	printf("list dessc:\n");
	list_for_each_reverse(iter, list)
	{
		printf("%s\n", (const char *) list_node_data(iter));
	}

	list_remove_node(list, list_first(list));
	list_remove_node(list, list_last(list));

	list_destroy(list);
}END_TEST

static bool data_freed = false;
static void data_free(void *data) {
	data_freed = true;
}

START_TEST(test_list_destroy_with_data)
{
	data_freed = false;
	const char *data = "foobar";
	list_t *list = list_create();
	list_append(list, (void *) data);
	list_destroy_with_data(list, data_free);
	ck_assert_int_eq(data_freed, 1);
}END_TEST

START_TEST(test_list_destroy_node_with_data)
{
	data_freed = false;
	const char *data = "foobar";
	list_t *list = list_create();
	list_append(list, (void *) data);
	ck_assert_int_eq(list_count(list), 1);

	list_node_t *node = list_first(list);
	ck_assert_int_eq(list_remove_node_with_data(list, node, data_free), 1);
	ck_assert_int_eq(data_freed, 1);
	ck_assert_int_eq(list_count(list), 0);
	list_destroy(list);
}END_TEST

START_TEST(test_list_destroy_non_existing_node)
{
	const char *data = "foobar";
	list_t *list = list_create();
	ck_assert_int_eq(list_remove_node(list, 0xdeadbeef), 0);
	list_destroy(list);
}END_TEST

START_TEST(test_list_append_data_check_first_last)
{
	const char *data = "foo";

	list_t *list = list_create();
	ck_assert_ptr_ne(list, NULL);
	list_append(list, (void *) data);

	list_node_t *node = list_first(list);
	ck_assert_str_eq((const char *)list_node_data(node), "foo");

	list_node_t *node2 = list_last(list);
	ck_assert_str_eq((const char *)list_node_data(node2), "foo");

	list_destroy(list);
}END_TEST

START_TEST(test_list_prepend_data_check_first_last)
{
	const char *data = "foo";

	list_t *list = list_create();
	ck_assert_ptr_ne(list, NULL);
	list_prepend(list, (void *) data);

	list_node_t *node = list_first(list);
	ck_assert_str_eq((const char *)list_node_data(node), "foo");

	list_node_t *node2 = list_last(list);
	ck_assert_str_eq((const char *)list_node_data(node2), "foo");

	list_destroy(list);
}END_TEST

START_TEST(test_list_remove_node)
{
	const char *data1 = "foo";

	list_t *list = list_create();
	list_append(list, (void *) data1);

	list_remove_node(list, list_first(list));

	list_destroy(list);
}END_TEST

int main(void)
{
	int number_failed;
	Suite *s = suite_create("bus_list");
	TCase *tc_core = tcase_create("Core");
	tcase_set_timeout(tc_core, 1.0);

	tcase_add_test(tc_core, test_list_create_destroy);
	tcase_add_test(tc_core, test_list_destroy_with_data);
	tcase_add_test(tc_core, test_list_destroy_node_with_data);
	tcase_add_test(tc_core, test_list_destroy_non_existing_node);
	tcase_add_test(tc_core, test_list_append_data_check_first_last);
	tcase_add_test(tc_core, test_list_prepend_data_check_first_last);
	tcase_add_test(tc_core, test_list_remove_node);

	suite_add_tcase(s, tc_core);
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

