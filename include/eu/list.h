/*
 * object_list.h
 *
 *  Created on: Apr 9, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_EU_LIST_H_
#define INCLUDE_EU_LIST_H_

#include <stdbool.h>
#include "eu/types.h"

// free node function typedef
typedef void (*eu_list_destroy_element_fn_t)(void *element);

// loop all elements
#define eu_list_for_each(node, list) \
		for(node = eu_list_first(list); node; node = eu_list_node_next(node))

#define eu_list_for_each_declare(node, list) \
		for(eu_list_node_t *node = eu_list_first(list); node; node = eu_list_node_next(node))

// loop all elements reverse
#define eu_list_for_each_reverse(node, list) \
		for(node = eu_list_last(list); node; node = eu_list_node_prev(node))

#define eu_list_for_each_reverse_declare(node, list) \
		for(eu_list_node_t *node = eu_list_last(list); node; node = eu_list_node_prev(node))

// list functions
eu_list_t *eu_list_create(void);
void eu_list_destroy(eu_list_t *list);
void eu_list_destroy_with_data(eu_list_t *list, eu_list_destroy_element_fn_t destroy_cb);
size_t eu_list_count(eu_list_t *list);
eu_list_node_t *eu_list_append(eu_list_t *list, void *element);
eu_list_node_t *eu_list_prepend(eu_list_t *list, void *element);
eu_list_node_t *eu_list_first(eu_list_t *list);
eu_list_node_t *eu_list_last(eu_list_t *list);
bool eu_list_remove_node(eu_list_t *list, eu_list_node_t *node);
bool eu_list_remove_node_with_data(eu_list_t *list, eu_list_node_t *node, eu_list_destroy_element_fn_t destroy_cb);

// node functions
eu_list_node_t *eu_list_node_next(eu_list_node_t *list);
eu_list_node_t *eu_list_node_prev(eu_list_node_t *list);
void *eu_list_node_data(eu_list_node_t *node);

#endif /* INCLUDE_EU_OBJECT_LIST_H_ */
