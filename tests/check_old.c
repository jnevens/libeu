#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

#include "bus/object.h"
#include "bus/parameter.h"
#include "bus/variant.h"
#include "bus/list.h"
#include "bus/string.h"
#include "bus/log.h"
#include "bus/event.h"
#include "bus/timer.h"

static const char *version = VERSION;

void test_list(void)
{
	const char *data1 = "foo";
	const char *data2 = "bar";
	const char *data3 = "boo";
	const char *data4 = "baa";

	list_t *list = list_create();
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
}

void test_list_2(void)
{
	const char *data1 = "foo";

	list_t *list = list_create();
	list_append(list, (void *) data1);

	list_remove_node(list, list_first(list));

	list_destroy(list);
}

void test_parameter(void)
{
	object_t *root = object_create(NULL, "com");

	parameter_create(root, "https", PARAMETER_TYPE_BOOL);
	object_parameter_set_bool(root, "https", false);

	object_destroy(root);
}

void test_variant(void)
{
	variant_t *var1 = variant_create(VARIANT_TYPE_BOOL);
	variant_set_bool(var1, true);
	if (variant_bool(var1) != true)
		printf("ERROR\n");
	variant_destroy(var1);
}

void test_tree(void)
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
}

void event_in(int fd, void *arg)
{
	char buf[200] = { 0 };
	if (read(fd, buf, 200) > 0) {
		size_t len = strlen(buf);
		buf[len - 1] = '\0';
		log_info("buf: %s", buf);
	}
}

void test_event(void)
{
	if (!event_loop_init())
		log_err("event_init failed!");

	if (!event_add(0, event_in, NULL, (void *) "test")) {
		log_err("failed adding event stdin");
	}

	event_loop();
	event_loop_cleanup();
}

static bool timer2_cb(void *arg)
{
	static int count = 10;
	log_info("Timer 2 done! %llu", time_ms());
	bool keep_running = (--count) ? event_timer_continue : event_timer_stop;
	if (keep_running == event_timer_stop) {
		event_loop_stop();
	}
	return keep_running;
}

static bool timer1_cb(void *arg)
{
	static int count = 5;
	log_info("Timer 1 done! %llu", time_ms());
	bool keep_running = (--count) ? event_timer_continue : event_timer_stop;
	if (keep_running == event_timer_stop) {
		event_loop_stop();
	}
	return keep_running;
}

void test_timer(void)
{
	event_loop_init();

	event_timer_create(10, timer1_cb, NULL);
	event_timer_create(400, timer2_cb, NULL);

	log_debug("start event loop");
	event_loop();
	event_loop_cleanup();
}

void test_string(void) {
	string_t *str = string_create();
	log_debug("string: %s", string_to_da_char(str));
	string_from_char(str, "test");
	log_debug("string: %s", string_to_da_char(str));
	if(string_length(str) != 4)
		log_err("invalid lengt of string!");

	string_destroy(str);
}

int main(int argc, char *argv[])
{
	log_init("bus-test");
	log_set_print_level(BLOG_DEBUG);
	log_set_syslog_level(BLOG_NOLOG);
	log_info("bus v%s", version);

	if (strcmp(argv[1], "tree") == 0) {
		test_tree();
	} else if (strcmp(argv[1], "list") == 0) {
		test_list();
		test_list_2();
	} else if (strcmp(argv[1], "variant") == 0) {
		test_variant();
	} else if (strcmp(argv[1], "parameter") == 0) {
		test_parameter();
	} else if (strcmp(argv[1], "event") == 0) {
		test_event();
	} else if (strcmp(argv[1], "timer") == 0) {
		test_timer();
	} else if (strcmp(argv[1], "string") == 0) {
		test_string();
	}

	return 0;
}

