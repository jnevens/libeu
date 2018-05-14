/*
 * check_event.c
 *
 *  Created on: Apr 29, 2018
 *      Author: jnevens
 */
#include <check.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

#include <eu/event.h>
#include <eu/log.h>

static int t1_count = 0;
static int t2_count = 0;
static int t3_count = 0;

static bool t3_callback(void *arg)
{
	t3_count++;
	return false;
}

static bool t1_callback(void *arg)
{
	t1_count++;
	eu_event_loop_stop();
	return false;
}

static bool t2_callback(void *arg)
{
	t2_count++;
	for (int i = 1; i <= 200; i++) {
		eu_event_timer_t *t = eu_event_timer_create(i/10 + 1, t3_callback, NULL);
		ck_assert_ptr_ne(t, NULL);
	}
	return false;
}

START_TEST(test_event_create_destroy)
{
	eu_event_loop_init();

	eu_event_timer_t *t1 = eu_event_timer_create(200, t1_callback, NULL);
	ck_assert_ptr_ne(t1, NULL);
	eu_event_timer_t *t2 = eu_event_timer_create(2, t2_callback, NULL);
	ck_assert_ptr_ne(t2, NULL);

	t1_count = 0;
	t2_count = 0;
	t3_count = 0;

	eu_event_loop();

	ck_assert_int_eq(t1_count, 1);
	ck_assert_int_eq(t2_count, 1);
	ck_assert_int_eq(t3_count, 200);

	eu_event_loop_cleanup();
}END_TEST

int main(void)
{
	int number_failed;
	Suite *s = suite_create("eu_event");
	TCase *tc_core = tcase_create("Core");
	tcase_set_timeout(tc_core, 1.0);

	tcase_add_test(tc_core, test_event_create_destroy);

	eu_log_init("unittests_eu_event");

	suite_add_tcase(s, tc_core);
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

