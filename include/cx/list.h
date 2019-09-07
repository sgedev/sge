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

static inline void cx_list_node_reset(cx_list_node_t *node)
{
	CX_ASSERT(node != NULL);

	node->prev = node;
	node->next = node;
}

static inline void cx_list_node_link(
	cx_list_node_t *node, cx_list_node_t *prev, cx_list_node_t *next)
{
	CX_ASSERT(node != NULL);
	CX_ASSERT(prev != NULL);
	CX_ASSERT(next != NULL);

	node->prev = prev;
	node->next = next;
	prev->next = node;
	next->prev = node;
}

static inline void cx_list_node_unlink(cx_list_node_t *node)
{
	CX_ASSERT(node != NULL);
	CX_ASSERT(node->prev != NULL);
	CX_ASSERT(node->next != NULL);

	node->prev->next = node->next;
	node->next->prev = node->prev;

	cx_list_node_reset(node);
}

static inline int cx_list_node_linked(const cx_list_node_t *node)
{
	CX_ASSERT(node != NULL);

	return (node != node->next);
}

static inline cx_list_node_t *cx_list_node_prev(cx_list_node_t *node)
{
	CX_ASSERT(node != NULL);
	CX_ASSERT(node->prev != NULL);

	return node->prev;
}

static inline cx_list_node_t *cx_list_node_next(cx_list_node_t *node)
{
	CX_ASSERT(node != NULL);
	CX_ASSERT(node->next != NULL);

	return node->next;
}

typedef struct {
	cx_list_node_t knot;
} cx_list_t;

CX_API int cx_list_length(cx_list_t *list);
CX_API int cx_list_has(cx_list_t *list, cx_list_node_t *node);

static inline void cx_list_reset(cx_list_t *list)
{
	CX_ASSERT(list != NULL);

	cx_list_node_reset(&list->knot);
}

static inline int cx_list_empty(const cx_list_t *list)
{
	CX_ASSERT(list != NULL);

	return !cx_list_node_linked(&list->knot);
}

static inline cx_list_node_t *cx_list_knot(cx_list_t *list)
{
	CX_ASSERT(list != NULL);

	return &list->knot;
}

static inline cx_list_node_t *cx_list_head(cx_list_t *list)
{
	CX_ASSERT(list != NULL);

	return cx_list_node_next(&list->knot);
}

static inline cx_list_node_t *cx_list_tail(cx_list_t *list)
{
	CX_ASSERT(list != NULL);

	return cx_list_node_prev(&list->knot);
}

static inline cx_list_node_t *cx_list_del_head(cx_list_t *list)
{
	cx_list_node_t *node;

	CX_ASSERT(list != NULL);

	node = cx_list_head(list);
	cx_list_node_unlink(node);

	return node;
}

static inline cx_list_node_t *cx_list_del_tail(cx_list_t *list)
{
	cx_list_node_t *node;

	CX_ASSERT(list != NULL);

	node = cx_list_tail(list);
	cx_list_node_unlink(node);

	return node;
}

static inline void cx_list_del_node(cx_list_t *list, cx_list_node_t *node)
{
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);
	CX_ASSERT(cx_list_has(list, node));

	cx_list_node_unlink(node);
}

static inline void cx_list_clear(cx_list_t *list)
{
	CX_ASSERT(list != NULL);

	while (!cx_list_empty(list))
		cx_list_del_head(list);
}

static inline void cx_list_add_head(cx_list_t *list, cx_list_node_t *node)
{
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);

	cx_list_node_unlink(node);
	cx_list_node_link(node, cx_list_knot(list), cx_list_head(list));	
}

static inline void cx_list_add_tail(cx_list_t *list, cx_list_node_t *node)
{
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);

	cx_list_node_unlink(node);
	cx_list_node_link(node, cx_list_tail(list), cx_list_knot(list));	
}

CX_END_DECLS

#define CX_LIST_FOREACH(pnode, plist) \
	for ((pnode)  = cx_list_head(plist); \
		 (pnode) != cx_list_knot(plist); \
		 (pnode)  = cx_list_node_next(pnode))

#endif /* CX_LIST_H */

