#include <check.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

#include <json-c/json.h>

#include <eu/variant.h>
#include <eu/variant_map.h>

START_TEST(test_variant_map_create_destroy)
{
	eu_variant_map_t *map = eu_variant_map_create();
	ck_assert_ptr_ne(map, NULL);
	eu_variant_map_destroy(map);
}END_TEST

START_TEST(test_variant_map_add_remove_find)
{
	eu_variant_map_t *map = eu_variant_map_create();

	ck_assert_int_eq(eu_variant_map_is_empty(map), 1);

	eu_variant_map_set_bool(map, "isBool", true);
	eu_variant_map_set_double(map, "isDouble", 199.34);

	ck_assert_int_eq(eu_variant_map_count(map), 2);
	ck_assert_int_eq(eu_variant_map_is_empty(map), 0);

	ck_assert_int_eq(eu_variant_map_has(map, "isBool"), true);
	ck_assert_int_eq(eu_variant_map_has(map, "isDouble"), true);
	ck_assert_int_eq(eu_variant_map_has(map, "foobar"), false);

	eu_variant_map_delete(map, "isDouble");
	ck_assert_int_eq(eu_variant_map_has(map, "isDouble"), false);

	eu_variant_map_destroy(map);
}END_TEST

START_TEST(test_variant_map_foreach)
{
	eu_variant_map_t *map = eu_variant_map_create();
	eu_variant_map_set_bool(map, "isBool", true);
	eu_variant_map_set_double(map, "isDouble", 199.34);

	int i = 0;
	eu_variant_map_for_each_pair(pair, map) {
		if (i == 0) {
			ck_assert_str_eq(eu_variant_map_pair_get_key(pair), "isBool");
		} else if (i == 1) {
			ck_assert_str_eq(eu_variant_map_pair_get_key(pair), "isDouble");
		}
		i++;
	}

	eu_variant_map_destroy(map);
}END_TEST

START_TEST(test_variant_map_foreach_reverse)
{
	eu_variant_map_t *map = eu_variant_map_create();
	eu_variant_map_set_bool(map, "isBool", true);
	eu_variant_map_set_uint8(map, "isDouble", 199);

	int i = 0;
	eu_variant_map_for_each_pair_reverse(pair, map) {
		if (i == 0) {
			ck_assert_str_eq(eu_variant_map_pair_get_key(pair), "isDouble");
			eu_variant_t *var = eu_variant_map_pair_get_val(pair);
			ck_assert_ptr_ne(var, NULL);
			ck_assert_uint_eq(eu_variant_uint8(var), 199);
			ck_assert_ptr_eq(eu_variant_map_pair_get_map(pair), map);
		} else if (i == 1) {
			ck_assert_str_eq(eu_variant_map_pair_get_key(pair), "isBool");
			eu_variant_t *var = eu_variant_map_pair_get_val(pair);
			ck_assert_ptr_ne(var, NULL);
			ck_assert_int_eq(eu_variant_bool(var), true);
			ck_assert_ptr_eq(eu_variant_map_pair_get_map(pair), map);
		}
		i++;
	}

	eu_variant_map_destroy(map);
}END_TEST

START_TEST(test_variant_map_serialize_simple)
{
	eu_variant_t *var = eu_variant_create(EU_VARIANT_TYPE_MAP);
	eu_variant_map_t *map = eu_variant_map_create();
	eu_variant_set_map(var, map);
	eu_variant_map_set_bool(map, "isBool", true);
	eu_variant_map_set_uint8(map, "isDouble", 199);

	json_object *obj = eu_variant_serialize(var);

	const char *json = json_object_to_json_string(obj);
	ck_assert_str_eq(json, "{ \"type\": 14, \"value\": [ { \"name\": \"isBool\", \"value\": { \"type\": 1, \"value\": true } }, { \"name\": \"isDouble\", \"value\": { \"type\": 3, \"value\": 199 } } ] }");

	eu_variant_destroy(var);
	json_object_put(obj);
}END_TEST

START_TEST(test_variant_map_deserialize_simple)
{
	json_object *jobj = json_tokener_parse("{ \"type\": 14, \"value\": [ { \"name\": \"isBool\", \"value\": { \"type\": 1, \"value\": true } }, { \"name\": \"isDouble\", \"value\": { \"type\": 3, \"value\": 199 } } ] }");
	eu_variant_t *var = eu_variant_deserialize(jobj);
	eu_variant_map_t *map = eu_variant_da_map(var);
	ck_assert_int_eq(eu_variant_map_get_bool(map, "isBool"), 1);
	ck_assert_int_eq(eu_variant_map_get_uint8(map, "isDouble"), 199);

	eu_variant_destroy(var);
	json_object_put(jobj);
}END_TEST

int main(void)
{
	int number_failed;
	Suite *s = suite_create("eu_variant_map");
	TCase *tc_core = tcase_create("Core");
	tcase_set_timeout(tc_core, 1.0);

	tcase_add_test(tc_core, test_variant_map_create_destroy);
	tcase_add_test(tc_core, test_variant_map_add_remove_find);
	tcase_add_test(tc_core, test_variant_map_foreach);
	tcase_add_test(tc_core, test_variant_map_foreach_reverse);
	tcase_add_test(tc_core, test_variant_map_serialize_simple);
	tcase_add_test(tc_core, test_variant_map_deserialize_simple);

	suite_add_tcase(s, tc_core);
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

