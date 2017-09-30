#include <check.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

#include <eu/list.h>

START_TEST(test_list_create_destroy)
{
	const char *data1 = "foo";
	const char *data2 = "bar";
	const char *data3 = "boo";
	const char *data4 = "baa";

	eu_list_t *list = eu_list_create();
	ck_assert_ptr_ne(list, NULL);
	eu_list_append(list, (void *) data2);
	eu_list_prepend(list, (void *) data1);
	eu_list_append(list, (void *) data3);
	eu_list_append(list, (void *) data4);

	eu_list_node_t *iter = NULL;
	printf("list asc:\n");
	eu_list_for_each(iter, list)
	{
		printf("%s\n", (const char *) eu_list_node_data(iter));
	}

	printf("list dessc:\n");
	eu_list_for_each_reverse(iter, list)
	{
		printf("%s\n", (const char *) eu_list_node_data(iter));
	}

	eu_list_remove_node(list, eu_list_first(list));
	eu_list_remove_node(list, eu_list_last(list));

	eu_list_destroy(list);
}END_TEST

static bool data_freed = false;
static void data_free(void *data) {
	data_freed = true;
}

START_TEST(test_list_destroy_with_data)
{
	data_freed = false;
	const char *data = "foobar";
	eu_list_t *list = eu_list_create();
	eu_list_append(list, (void *) data);
	eu_list_destroy_with_data(list, data_free);
	ck_assert_int_eq(data_freed, 1);
}END_TEST

START_TEST(test_list_destroy_node_with_data)
{
	data_freed = false;
	const char *data = "foobar";
	eu_list_t *list = eu_list_create();
	eu_list_append(list, (void *) data);
	ck_assert_int_eq(eu_list_count(list), 1);

	eu_list_node_t *node = eu_list_first(list);
	ck_assert_int_eq(eu_list_remove_node_with_data(list, node, data_free), 1);
	ck_assert_int_eq(data_freed, 1);
	ck_assert_int_eq(eu_list_count(list), 0);
	eu_list_destroy(list);
}END_TEST

START_TEST(test_list_destroy_non_existing_node)
{
	const char *data = "foobar";
	eu_list_t *list = eu_list_create();
	ck_assert_int_eq(eu_list_remove_node(list, 0xdeadbeef), 0);
	eu_list_destroy(list);
}END_TEST

START_TEST(test_list_append_data_check_first_last)
{
	const char *data = "foo";

	eu_list_t *list = eu_list_create();
	ck_assert_ptr_ne(list, NULL);
	eu_list_append(list, (void *) data);

	eu_list_node_t *node = eu_list_first(list);
	ck_assert_str_eq((const char *)eu_list_node_data(node), "foo");

	eu_list_node_t *node2 = eu_list_last(list);
	ck_assert_str_eq((const char *)eu_list_node_data(node2), "foo");

	eu_list_destroy(list);
}END_TEST

START_TEST(test_list_prepend_data_check_first_last)
{
	const char *data = "foo";

	eu_list_t *list = eu_list_create();
	ck_assert_ptr_ne(list, NULL);
	eu_list_prepend(list, (void *) data);

	eu_list_node_t *node = eu_list_first(list);
	ck_assert_str_eq((const char *)eu_list_node_data(node), "foo");

	eu_list_node_t *node2 = eu_list_last(list);
	ck_assert_str_eq((const char *)eu_list_node_data(node2), "foo");

	eu_list_destroy(list);
}END_TEST

START_TEST(test_list_remove_node)
{
	const char *data1 = "foo";

	eu_list_t *list = eu_list_create();
	eu_list_append(list, (void *) data1);

	eu_list_remove_node(list, eu_list_first(list));

	eu_list_destroy(list);
}END_TEST

START_TEST(test_list_3_element_check)
{
	const char *data1 = "foo";
	const char *data2 = "bar";
	const char *data3 = "dead";

	eu_list_t *list = eu_list_create();
	eu_list_append(list, (void *) data1);
	eu_list_append(list, (void *) data2);
	eu_list_append(list, (void *) data3);

	ck_assert_str_eq(eu_list_node_data(eu_list_first(list)), "foo");
	ck_assert_str_eq(eu_list_node_data(eu_list_last(list)), "dead");
	ck_assert_str_eq(eu_list_node_data(eu_list_node_prev(eu_list_last(list))), "bar");
	ck_assert_str_eq(eu_list_node_data(eu_list_node_next(eu_list_first(list))), "bar");

	ck_assert_ptr_eq(eu_list_node_next(eu_list_last(list)), NULL);
	ck_assert_ptr_eq(eu_list_node_prev(eu_list_first(list)), NULL);

	eu_list_destroy(list);
}END_TEST

int main(void)
{
	int number_failed;
	Suite *s = suite_create("eu_list");
	TCase *tc_core = tcase_create("Core");
	tcase_set_timeout(tc_core, 1.0);

	tcase_add_test(tc_core, test_list_create_destroy);
	tcase_add_test(tc_core, test_list_destroy_with_data);
	tcase_add_test(tc_core, test_list_destroy_node_with_data);
	tcase_add_test(tc_core, test_list_destroy_non_existing_node);
	tcase_add_test(tc_core, test_list_append_data_check_first_last);
	tcase_add_test(tc_core, test_list_prepend_data_check_first_last);
	tcase_add_test(tc_core, test_list_remove_node);
	tcase_add_test(tc_core, test_list_3_element_check);

	suite_add_tcase(s, tc_core);
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

