/*
 * object_list.c
 *
 *  Created on: Apr 9, 2016
 *      Author: jnevens
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eu/list.h"

struct eu_list_node_s
{
	eu_list_t *list;
	struct eu_list_node_s *next;
	struct eu_list_node_s *prev;
	void *element;
};

struct eu_list_s
{
	size_t count;
	eu_list_node_t *head;
	eu_list_node_t *tail;
};

static eu_list_node_t *list_node_create(eu_list_t *list, void *element)
{
	eu_list_node_t *node = calloc(1, sizeof(eu_list_node_t));
	node->element = element;
	return node;
}

eu_list_t *eu_list_create(void)
{
	eu_list_t *list = calloc(1, sizeof(eu_list_t));
	list->count = 0;
	return list;
}

void eu_list_destroy(eu_list_t *list)
{
	eu_list_node_t *node = list->head;
	while(node) {
		eu_list_node_t *next = node->next;
		free(node);
		node = next;
	}
	free(list);
}

void eu_list_destroy_with_data(eu_list_t *list, eu_list_destroy_element_fn_t destroy_cb)
{
	eu_list_node_t *node = NULL;
	eu_list_for_each(node, list) {
		destroy_cb(eu_list_node_data(node));
	}
	eu_list_destroy(list);
}

size_t eu_list_count(eu_list_t *list)
{
	return list->count;
}

void eu_list_append(eu_list_t *list, void *element)
{
	eu_list_node_t *new = list_node_create(list, element);
	eu_list_node_t *last = list->tail;
	if(last) {
		new->prev = last;
		last->next = new;
	} else {
		list->head = new;
	}
	list->tail = new;
	list->count++;
}

void eu_list_prepend(eu_list_t *list, void *element)
{
	eu_list_node_t *new = list_node_create(list, element);
	eu_list_node_t *first = list->head;
	if(first) {
		new->next = first;
		first->prev = new;
	} else {
		list->tail = new;
	}
	list->head=new;
	list->count++;
}

eu_list_node_t *eu_list_first(eu_list_t *list)
{
	return list->head;
}

eu_list_node_t *eu_list_last(eu_list_t *list)
{
	return list->tail;
}

bool eu_list_remove_node(eu_list_t *list, eu_list_node_t *node)
{
	eu_list_node_t *lnode = NULL;
	bool found = false;

	eu_list_for_each(lnode, list)
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

bool eu_list_remove_node_with_data(eu_list_t *list, eu_list_node_t *node, eu_list_destroy_element_fn_t destroy_cb)
{
	if(destroy_cb) {
		destroy_cb(eu_list_node_data(node));
	}

	return eu_list_remove_node(list, node);
}

eu_list_node_t *eu_list_node_next(eu_list_node_t *node)
{
	return (node) ? node->next : NULL;
}

eu_list_node_t *eu_list_node_prev(eu_list_node_t *node)
{
	return (node) ? node->prev : NULL;
}

void *eu_list_node_data(eu_list_node_t *node)
{
	return node->element;
}
