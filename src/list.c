/*
 * object_list.c
 *
 *  Created on: Apr 9, 2016
 *      Author: jnevens
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bus/list.h"

struct list_node_s
{
	list_t *list;
	struct list_node_s *next;
	struct list_node_s *prev;
	void *element;
};

struct list_s
{
	size_t count;
	list_node_t *head;
	list_node_t *tail;
};

static list_node_t *list_node_create(list_t *list, void *element)
{
	list_node_t *node = calloc(1, sizeof(list_node_t));
	node->element = element;
	return node;
}

list_t *list_create(void)
{
	list_t *list = calloc(1, sizeof(list_t));
	list->count = 0;
	return list;
}

void list_destroy(list_t *list)
{
	list_node_t *node = list->head;
	while(node) {
		list_node_t *next = node->next;
		free(node);
		node = next;
	}
	free(list);
}

void list_destroy_with_data(list_t *list, list_destroy_element_fn_t destroy_cb)
{
	list_node_t *node = NULL;
	list_for_each(node, list) {
		destroy_cb(list_node_data(node));
	}
	list_destroy(list);
}

size_t list_count(list_t *list)
{
	return list->count;
}

void list_append(list_t *list, void *element)
{
	list_node_t *new = list_node_create(list, element);
	list_node_t *last = list->tail;
	if(last) {
		new->prev = last;
		last->next = new;
	} else {
		list->head = new;
	}
	list->tail = new;
	list->count++;
}

void list_prepend(list_t *list, void *element)
{
	list_node_t *new = list_node_create(list, element);
	list_node_t *first = list->head;
	if(first) {
		new->next = first;
		first->prev = new;
	} else {
		list->tail = new;
	}
	list->head=new;
	list->count++;
}

list_node_t *list_first(list_t *list)
{
	return list->head;
}

list_node_t *list_last(list_t *list)
{
	return list->tail;
}

bool list_remove_node(list_t *list, list_node_t *node)
{
	list_node_t *lnode = NULL;
	bool found = false;

	list_for_each(lnode, list)
	{
		if (lnode == node) {
			found = true;
			break;
		}
	}

	if (!found)
		return false;

	if (list->head == node) {
		list->head = node->next;
	}

	if (node->prev) {
		node->prev->next = node->next;
	}

	if (node->next) {
		node->next->prev = node->prev;
	}

	if (list->tail == node) {
		list->tail = node->prev;
	}
	free(node);
	list->count--;
	return true;
}

bool list_remove_node_with_data(list_t *list, list_node_t *node, list_destroy_element_fn_t destroy_cb)
{
	if(destroy_cb) {
		destroy_cb(list_node_data(node));
	}

	return list_remove_node(list, node);
}

list_node_t *list_node_next(list_node_t *node)
{
	return (node) ? node->next : NULL;
}

list_node_t *list_node_prev(list_node_t *node)
{
	return (node) ? node->prev : NULL;
}

void *list_node_data(list_node_t *node)
{
	return node->element;
}
