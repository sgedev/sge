/*
 *
 */
#ifndef SGE_LIST_H
#define SGE_LIST_H

#include "sge_common.h"

typedef struct sge_list_node_struct {
	struct sge_list_node_struct *prev;
	struct sge_list_node_struct *next;
} sge_list_node_t;

static inline void sge_list_node_reset(sge_list_node_t *node)
{
	SGE_ASSERT(node != NULL);

	node->prev = node;
	node->next = node;
}

static inline void sge_list_node_link(sge_list_node_t *node, sge_list_node_t *prev, sge_list_node_t *next)
{
	SGE_ASSERT(node != NULL);
	SGE_ASSERT(prev != NULL);
	SGE_ASSERT(next != NULL);

	node->prev = prev;
	node->next = next;
	prev->next = node;
	next->prev = node;
}

static inline void sge_list_node_unlink(sge_list_node_t *node)
{
	SGE_ASSERT(node != NULL);
	SGE_ASSERT(node->prev != NULL);
	SGE_ASSERT(node->next != NULL);

	node->prev->next = node->next;
	node->next->prev = node->prev;
	sge_list_node_reset(node);
}

static inline sge_bool_t sge_list_node_is_linked(sge_list_node_t *node)
{
	SGE_ASSERT(node != NULL);
	SGE_ASSERT(node->prev != NULL);
	SGE_ASSERT(node->next != NULL);

	return (node != node->next);
}

static inline sge_list_node_t *sge_list_node_get_prev(sge_list_node_t *node)
{
	SGE_ASSERT(node != NULL);
	SGE_ASSERT(node->prev != NULL);
	SGE_ASSERT(node->next != NULL);
	return node->prev;
}

static inline sge_list_node_t *sge_list_node_get_next(sge_list_node_t *node)
{
	SGE_ASSERT(node != NULL);
	SGE_ASSERT(node->prev != NULL);
	SGE_ASSERT(node->next != NULL);

	return node->next;
}

typedef struct {
	sge_list_node_t knot;
} sge_list_t;

int sge_list_get_length(sge_list_t *list);
sge_bool_t sge_list_has(sge_list_t *list, sge_list_node_t *node);

static inline void sge_list_reset(sge_list_t *list)
{
	SGE_ASSERT(list != NULL);

	sge_list_node_reset(&list->knot);
}

static inline sge_bool_t sge_list_is_empty(sge_list_t *list)
{
	SGE_ASSERT(list != NULL);

	return !sge_list_node_is_linked(&list->knot);
}

static inline sge_list_node_t *sge_list_get_knot(sge_list_t *list)
{
	SGE_ASSERT(list != NULL);

	return &list->knot;
}

static inline sge_list_node_t *sge_list_get_head(sge_list_t *list)
{
	SGE_ASSERT(list != NULL);

	return list->knot.next;
}

static inline sge_list_node_t *sge_list_get_tail(sge_list_t *list)
{
	SGE_ASSERT(list != NULL);

	return list->knot.prev;
}

static inline sge_list_node_t *sge_list_del_head(sge_list_t *list)
{
	sge_list_node_t *node;

	SGE_ASSERT(list != NULL);

	node = sge_list_get_head(list);
	sge_list_node_unlink(node);

	return node;
}

static inline sge_list_node_t *sge_list_del_tail(sge_list_t *list)
{
	sge_list_node_t *node;

	SGE_ASSERT(list != NULL);

	node = sge_list_get_tail(list);
	sge_list_node_unlink(node);

	return node;
}

static inline sge_list_node_t *sge_list_del(sge_list_t *list, sge_list_node_t *node)
{
	SGE_ASSERT(list != NULL);
	SGE_ASSERT(node != NULL);
	SGE_ASSERT(sge_list_has(list, node));

	sge_list_node_unlink(node);
}

static inline void sge_list_add_head(sge_list_t *list, sge_list_node_t *node)
{
	SGE_ASSERT(list != NULL);
	SGE_ASSERT(node != NULL);
	SGE_ASSERT(!sge_list_has(list, node));

	sge_list_node_link(node, sge_list_get_knot(list), sge_list_get_head(list));
}

static inline void sge_list_add_tail(sge_list_t *list, sge_list_node_t *node)
{
	SGE_ASSERT(list != NULL);
	SGE_ASSERT(node != NULL);
	SGE_ASSERT(!sge_list_has(list, node));

	sge_list_node_link(node, sge_list_get_tail(list), sge_list_get_knot(list));
}

#define SGE_LIST_FOREACH(np, lp) \
	for ((np) = sge_list_get_head(lp); (np) != NULL; (np) = sge_list_node_get_next(np))

#endif /* SGE_LIST_H */

