/*
 *
 */
#ifndef SGE_LIST_H
#define SGE_LIST_H

#include <sge/common.h>

SGE_C_BEGIN

typedef struct sge_list_node_tag {
	struct sge_list_node_tag *prev;
	struct sge_list_node_tag *next;
} sge_list_node;

static inline void sge_list_node_reset(sge_list_node *node)
{
	SGE_ASSERT(node != NULL);

	node->prev = node;
	node->next = node;
}

static inline void sge_list_node_link(sge_list_node *node, sge_list_node *prev, sge_list_node *next)
{
	SGE_ASSERT(node != NULL);
	SGE_ASSERT(prev != NULL);
	SGE_ASSERT(next != NULL);

	node->prev = prev;
	node->next = next;
	prev->next = node;
	next->prev = node;
}

static inline void sge_list_node_unlink(sge_list_node *node)
{
	SGE_ASSERT(node != NULL);
	SGE_ASSERT(node->prev != NULL);
	SGE_ASSERT(node->next != NULL);

	node->prev->next = node->next;
	node->next->prev = node->prev;

	sge_list_node_reset(node);
}

static inline int sge_list_node_linked(const sge_list_node *node)
{
	SGE_ASSERT(node != NULL);

	return (node != node->next);
}

static inline sge_list_node *sge_list_node_prev(sge_list_node *node)
{
	SGE_ASSERT(node != NULL);
	SGE_ASSERT(node->prev != NULL);

	return node->prev;
}

static inline sge_list_node *sge_list_node_next(sge_list_node *node)
{
	SGE_ASSERT(node != NULL);
	SGE_ASSERT(node->next != NULL);

	return node->next;
}

typedef struct {
	sge_list_node knot;
} sge_list;

int sge_list_length(sge_list *list);
int sge_list_has(sge_list *list, sge_list_node *node);

static inline void sge_list_reset(sge_list *list)
{
	SGE_ASSERT(list != NULL);

	sge_list_node_reset(&list->knot);
}

static inline int sge_list_empty(const sge_list *list)
{
	SGE_ASSERT(list != NULL);

	return !sge_list_node_linked(&list->knot);
}

static inline sge_list_node *sge_list_knot(sge_list *list)
{
	SGE_ASSERT(list != NULL);

	return &list->knot;
}

static inline sge_list_node *sge_list_head(sge_list *list)
{
	SGE_ASSERT(list != NULL);

	return sge_list_node_next(&list->knot);
}

static inline sge_list_node *sge_list_tail(sge_list *list)
{
	SGE_ASSERT(list != NULL);

	return sge_list_node_prev(&list->knot);
}

static inline sge_list_node *sge_list_del_head(sge_list *list)
{
	sge_list_node *node;

	SGE_ASSERT(list != NULL);

	node = sge_list_head(list);
	sge_list_node_unlink(node);

	return node;
}

static inline sge_list_node *sge_list_del_tail(sge_list *list)
{
	sge_list_node *node;

	SGE_ASSERT(list != NULL);

	node = sge_list_tail(list);
	sge_list_node_unlink(node);

	return node;
}

static inline void sge_list_del(sge_list *list, sge_list_node *node)
{
	SGE_ASSERT(list != NULL);
	SGE_ASSERT(node != NULL);
	SGE_ASSERT(sge_list_has(list, node));

	sge_list_node_unlink(node);
}

static inline void sge_list_add_head(sge_list *list, sge_list_node *node)
{
	SGE_ASSERT(list != NULL);
	SGE_ASSERT(node != NULL);

	sge_list_node_unlink(node);
	sge_list_node_link(node, sge_list_knot(list), sge_list_head(list));	
}

static inline void sge_list_add_tail(sge_list *list, sge_list_node *node)
{
	SGE_ASSERT(list != NULL);
	SGE_ASSERT(node != NULL);

	sge_list_node_unlink(node);
	sge_list_node_link(node, sge_list_tail(list), sge_list_knot(list));	
}

SGE_C_END

#define SGE_LIST_FOREACH(pnode, plist) \
	for ((pnode)  = sge_list_head(plist); \
		 (pnode) != sge_list_knot(plist); \
		 (pnode)  = sge_list_node_next(pnode))

#define SGE_LIST_FOREACH_R(pnode, plist) \
	for ((pnode)  = sge_list_tail(plist); \
		 (pnode) != sge_list_knot(plist); \
		 (pnode)  = sge_list_node_prev(pnode))

#endif /* SGE_LIST_H */

