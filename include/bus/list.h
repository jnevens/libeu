/*
 * object_list.h
 *
 *  Created on: Apr 9, 2016
 *      Author: jnevens
 */

#ifndef INCLUDE_BUS_LIST_H_
#define INCLUDE_BUS_LIST_H_

#include <stdbool.h>
#include "bus/types.h"

// free node function typedef
typedef void (*list_destroy_element_fn_t)(void *element);

// loop all elements
#define list_for_each(node, list) \
		for(node = list_first(list); node; node = list_node_next(node))

// loop all elements reverse
#define list_for_each_reverse(node, list) \
		for(node = list_last(list); node; node = list_node_prev(node))

// list functions
list_t *list_create(void);
void list_destroy(list_t *list);
void list_destroy_with_data(list_t *list, list_destroy_element_fn_t destroy_cb);
size_t list_count(list_t *list);
void list_append(list_t *list, void *element);
void list_prepend(list_t *list, void *element);
list_node_t *list_first(list_t *list);
list_node_t *list_last(list_t *list);
bool list_remove_node(list_t *list, list_node_t *node);
bool list_remove_node_with_data(list_t *list, list_node_t *node, list_destroy_element_fn_t destroy_cb);

// node functions
list_node_t *list_node_next(list_node_t *list);
list_node_t *list_node_prev(list_node_t *list);
void *list_node_data(list_node_t *node);

#endif /* INCLUDE_BUS_OBJECT_LIST_H_ */
