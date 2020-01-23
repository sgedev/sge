/*
 *
 */
#ifndef CX_LIST_H
#define CX_LIST_H

#include <cx/common.h>

CX_BEGIN_DECLS

typedef struct cx_list_node {
	struct cx_list_node *prev;
	struct cx_list_node *next;
} cx_list_node_t;

static CX_INLINE void cx_list_node_reset(cx_list_node_t *node)
{
	CX_ASSERT(node != NULL);
	node->prev = node;
	node->next = node;
}

static CX_INLINE int cx_list_node_is_linked(cx_list_node_t *node)
{
	CX_ASSERT(node != NULL);
	return (node->next != node);
}

static CX_INLINE void cx_list_node_link(cx_list_node_t *node, cx_list_node_t *prev, cx_list_node_t *next)
{
	CX_ASSERT(node != NULL);
	CX_ASSERT(prev != NULL);
	CX_ASSERT(next != NULL);
	node->prev = prev;
	node->next = next;
	prev->next = node;
	next->prev = node;
}

static CX_INLINE void cx_list_node_unlink(cx_list_node_t *node)
{
	CX_ASSERT(node != NULL);
	CX_ASSERT(node->prev != NULL);
	CX_ASSERT(node->next != NULL);
	node->prev->next = node->next;
	node->next->prev = node->prev;
	cx_list_node_reset(node);
}

static CX_INLINE cx_list_node_t *cx_list_node_get_prev(cx_list_node_t *node)
{
	CX_ASSERT(node != NULL);
	CX_ASSERT(node->prev != NULL);
	return node->prev;
}

static CX_INLINE cx_list_node_t *cx_list_node_get_next(cx_list_node_t *node)
{
	CX_ASSERT(node != NULL);
	CX_ASSERT(node->next != NULL);
	return node->next;
}

typedef struct {
	cx_list_node_t knot;
} cx_list_t;

#define CX_LIST_FOREACH(np, lp) \
	for ((np) = cx_list_get_head(lp); (np) != cx_list_get_knot(lp); (np) = cx_list_node_get_next(np))

static CX_INLINE void cx_list_reset(cx_list_t *list)
{
	CX_ASSERT(list != NULL);
	cx_list_node_reset(&list->knot);
}

static CX_INLINE int cx_list_is_empty(cx_list_t *list)
{
	CX_ASSERT(list != NULL);
	return !cx_list_node_is_linked(&list->knot);
}

static CX_INLINE cx_list_node_t *cx_list_get_head(cx_list_t *list)
{
	CX_ASSERT(list != NULL);
	return list->knot.next;
}

static CX_INLINE cx_list_node_t *cx_list_get_tail(cx_list_t *list)
{
	CX_ASSERT(list != NULL);
	return list->knot.prev;
}

static CX_INLINE cx_list_node_t *cx_list_get_knot(cx_list_t *list)
{
	CX_ASSERT(list != NULL);
	return &list->knot;
}

static CX_INLINE int cx_list_get_length(cx_list_t *list)
{
	int n = 0;
	cx_list_node_t *p;

	CX_ASSERT(list != NULL);

	CX_LIST_FOREACH(p, list)
		n += 1;

	return n;
}

static CX_INLINE void cx_list_add_head(cx_list_t *list, cx_list_node_t *node)
{
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);
	cx_list_node_link(node, cx_list_get_knot(list), cx_list_get_head(list));
}

static CX_INLINE void cx_list_add_tail(cx_list_t *list, cx_list_node_t *node)
{
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);
	cx_list_node_link(node, cx_list_get_tail(list), cx_list_get_knot(list));
}

static CX_INLINE cx_list_node_t *cx_list_del_head(cx_list_t *list)
{
	cx_list_node_t *node;

	CX_ASSERT(list != NULL);
	node = cx_list_get_head(list);
	cx_list_node_unlink(node);

	return node;
}

static CX_INLINE cx_list_node_t *cx_list_del_tail(cx_list_t *list)
{
	cx_list_node_t *node;

	CX_ASSERT(list != NULL);
	node = cx_list_get_tail(list);
	cx_list_node_unlink(node);

	return node;
}

static CX_INLINE int cx_list_contains(cx_list_t *list, cx_list_node_t *node)
{
	cx_list_node_t *p;

	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);

	CX_LIST_FOREACH(p, list) {
		if (node == p)
			return 1;
	}

	return 0;
}

static CX_INLINE void cx_list_del_node(cx_list_t *list, cx_list_node_t *node)
{
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);
	CX_ASSERT(cx_list_contains(list, node));

	cx_list_node_unlink(node);
}

CX_END_DECLS

#endif /* CX_LIST_H */
