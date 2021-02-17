#include <pthread.h>
#include <stdlib.h>
#include <stdio.h> // TODO: Remove after fprintf() removed

#include "llist.h"

struct node_t {
	struct node_t *next;
	void *data;
};

struct llist_t {
	struct node_t  *head;
	struct node_t  *tail;
	del_f           del;
	pthread_mutex_t lock;
};

// Deletes all nodes
// WARNING: NOT THREAD SAFE! Relies on calling function to lock
//
static void llist_delete_nodes(llist_t *llist);

llist_t *llist_create(del_f del)
{
	llist_t *llist = malloc(sizeof(*llist));
	if (!llist) {
		return NULL;
	}

	llist->head = NULL;
	llist->tail = NULL;
	llist->del  = del;
	pthread_mutex_init(&llist->lock, NULL);

	return llist;
} /* llist_create() */

void llist_delete(llist_t **p_llist)
{
	if (!p_llist) {
		return;
	}

	pthread_mutex_lock(&(*p_llist)->lock);
	{
		llist_delete_nodes(*p_llist);
	}
	pthread_mutex_unlock(&(*p_llist)->lock);
	pthread_mutex_destroy(&(*p_llist)->lock);

	(*p_llist)->head = NULL;
	(*p_llist)->tail = NULL;
	(*p_llist)->del = NULL;
	free(*p_llist);
	*p_llist = NULL;
} /* llist_delete() */

static void llist_delete_nodes(llist_t *llist)
{
	struct node_t *node = llist->head;
	struct node_t *temp = NULL;

	while (node) {
		temp = node;
		node = node->next;

        // TODO: Refactor to reduce number of evals
		if (llist->del) {
			llist->del(temp->data);
		}
		temp->data = NULL;
		temp->next = NULL;
		free(temp);
		temp = NULL;
	}
} /* llist_delete_nodes() */

bool llist_enqueue(llist_t *llist, void *data)
{
	if (!llist || !data) {
		return false;
	}

	struct node_t *node = malloc(sizeof(*node));
	if (!node) {
		return false;
	}

	node->data = data;
	node->next = NULL;

	pthread_mutex_lock(&llist->lock);
	{
		if (llist->tail) {
			llist->tail->next = node;
		} else {
			llist->head = node;
		}

		llist->tail = node;
	}
	pthread_mutex_unlock(&llist->lock);

	return true;
} /* llist_enqueue() */

bool llist_dequeue(llist_t *llist, void **data)
{
	bool ret = false;
	if (!data) {
		printf("Always false\n");
		return ret;
	}

	*data = NULL;
	if (!llist) {
		return ret;
	}

	pthread_mutex_lock(&llist->lock);
	{
		if (llist->head) {
			struct node_t *temp = llist->head;
			llist->head = temp->next;
			*data = temp->data;

			if (!llist->head) {
				llist->tail = NULL;
			}

			temp->next = NULL;
			temp->data = NULL;
			free(temp);
			temp = NULL;

			ret = true;
		}
	}
	pthread_mutex_unlock(&llist->lock);

	return ret;
} /* llist_dequeue() */

bool llist_push(llist_t *llist, void *data)
{
	if (!llist || !data) {
		return false;
	}

	struct node_t *node = malloc(sizeof(*node));
	if (!node) {
		return false;
	}
	node->data = data;

	pthread_mutex_lock(&llist->lock);
	{
		node->next = llist->head;
		llist->head = node;
	}
	pthread_mutex_unlock(&llist->lock);

	return true;
} /* llist_push() */

bool llist_pop(llist_t *llist, void **data)
{
	return llist_dequeue(llist, data);
} /* llist_pop() */

bool llist_is_empty(llist_t *llist)
{
	if (!llist || llist->head) {
		return false;
	}
	return true;
}