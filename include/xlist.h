/*
 *
 */
#ifndef XLIST_H
#define XLIST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xlist_node {
	struct xlist_node *prev;
	struct xlist_node *next;
} xlist_node_t;

static inline void xlist_node_reset(xlist_node_t *node)
{
	node->prev = node;
	node->next = node;
}

static inline void xlist_node_link(xlist_node_t *node, xlist_node_t *prev, xlist_node_t *next)
{	
	node->prev = prev;
	node->next = next;
	prev->next = node;
	next->prev = node;
}

static inline void xlist_node_unlink(xlist_node_t *node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
	node->prev = node;
	node->next = node;
}

static inline int xlist_node_linked(xlist_node_t *node)
{
	return (node->next != node);
}

static inline xlist_node_t *xlist_node_prev(xlist_node_t *node)
{
	return node->prev;
}

static inline xlist_node_t *xlist_node_next(xlist_node_t *node)
{
	return node->next;
}

typedef struct {
	xlist_node_t knot;
} xlist_t;

static inline void xlist_reset(xlist_t *list)
{
	xlist_node_reset(&list->knot);
}

static inline int xlist_empty(xlist_t *list)
{
	return !xlist_node_linked(&list->knot);
}

static inline xlist_node_t *xlist_knot(xlist_t *list)
{
	return &list->knot;
}

static inline xlist_node_t *xlist_head(xlist_t *list)
{
	return xlist_node_next(&list->knot);
}

static inline xlist_node_t *xlist_tail(xlist_t *list)
{
	return xlist_node_prev(&list->knot);
}

static inline void xlist_add_head(xlist_t *list, xlist_node_t *node)
{
	xlist_node_link(node, xlist_knot(list), xlist_head(list));
}

static inline void xlist_add_tail(xlist_t *list, xlist_node_t *node)
{
	xlist_node_link(node, xlist_tail(list), xlist_knot(list));
}

static inline xlist_node_t *xlist_del_head(xlist_t *list)
{
	xlist_node_t *node = xlist_head(list);
	xlist_node_unlink(node);

	return node;
}

static inline xlist_node_t *xlist_del_tail(xlist_t *list)
{
	xlist_node_t *node = xlist_tail(list);
	xlist_node_unlink(node);

	return node;
}

#define XLIST_FOREACH(nodep, listp) \
	for ((nodep) = xlist_head(listp); (nodep) != xlist_knot(listp); nodep = xlist_node_next(nodep))

static inline int xlist_has(xlist_t *list, xlist_node_t *node)
{
	xlist_node_t *p;

	XLIST_FOREACH(p, list) {
		if (p == node)
			return 1;
	}

	return 0;
}

static inline void xlist_del(xlist_t *list, xlist_node_t *node)
{
	if (xlist_has(list, node))
		xlist_node_unlink(node);
}

static inline int xlist_length(xlist_t *list)
{
	int length = 0;
	xlist_node_t *node;

	XLIST_FOREACH(node, list)
		length += 1;

	return length;
}

#ifdef __cplusplus
}
#endif

#endif /* XLIST_H */

