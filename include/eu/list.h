/*
 * object_list.h
 *
 *  Created on: Apr 9, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_BUS_LIST_H_
#define INCLUDE_BUS_LIST_H_

#include <stdbool.h>
#include "eu/types.h"

// free node function typedef
typedef void (*list_destroy_element_fn_t)(void *element);

// loop all elements
#define list_for_each(node, list) \
		for(node = list_first(list); node; node = list_node_next(node))

// loop all elements reverse
#define list_for_each_reverse(node, list) \
		for(node = list_last(list); node; node = list_node_prev(node))

// list functions
eu_list_t *list_create(void);
void list_destroy(eu_list_t *list);
void list_destroy_with_data(eu_list_t *list, list_destroy_element_fn_t destroy_cb);
size_t list_count(eu_list_t *list);
void list_append(eu_list_t *list, void *element);
void list_prepend(eu_list_t *list, void *element);
eu_list_node_t *list_first(eu_list_t *list);
eu_list_node_t *list_last(eu_list_t *list);
bool list_remove_node(eu_list_t *list, eu_list_node_t *node);
bool list_remove_node_with_data(eu_list_t *list, eu_list_node_t *node, list_destroy_element_fn_t destroy_cb);

// node functions
eu_list_node_t *list_node_next(eu_list_node_t *list);
eu_list_node_t *list_node_prev(eu_list_node_t *list);
void *list_node_data(eu_list_node_t *node);

#endif /* INCLUDE_BUS_OBJECT_LIST_H_ */
