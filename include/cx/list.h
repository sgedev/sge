/*
 *
 */
#ifndef CX_LIST_H
#define CX_LIST_H

#include <cx/common.h>

CX_BEGIN_DECLS

typedef struct cx_list_node_struct {
	struct cx_list_node_struct* prev;
	struct cx_list_node_struct* next;
} cx_list_node_t;

CX_FORCE_INLINE void cx_list_node_reset(cx_list_node_t* node) {
	CX_ASSERT(node != NULL);
	node->prev = node;
	node->next = node;
}

CX_FORCE_INLINE bool cx_list_node_is_linked(const cx_list_node_t* node) {
	CX_ASSERT(node != NULL);
	return (node != node->next);
}

CX_FORCE_INLINE void cx_list_node_link(cx_list_node_t* node, cx_list_node_t* prev, cx_list_node_t* next) {
	CX_ASSERT(node != NULL);
	CX_ASSERT(prev != NULL);
	CX_ASSERT(next != NULL);
	node->prev = prev;
	node->next = next;
	prev->next = node;
	next->prev = prev;
}

CX_FORCE_INLINE void cx_list_node_unlink(cx_list_node_t* node) {
	CX_ASSERT(node != NULL);
	CX_ASSERT(node->prev != NULL);
	CX_ASSERT(node->next != NULL);
	node->prev->next = node->next;
	node->next->prev = node->prev;
	node->prev = node;
	node->next = node;
}

CX_FORCE_INLINE cx_list_node_t* cx_list_node_prev(cx_list_node_t* node) {
	CX_ASSERT(node != NULL);
	CX_ASSERT(node->prev != NULL);
	return node->prev;
}

CX_FORCE_INLINE cx_list_node_t* cx_list_node_next(cx_list_node_t* node) {
	CX_ASSERT(node != NULL);
	CX_ASSERT(node->next != NULL);
	return node->next;
}

typedef struct {
	cx_list_node_t knot;
} cx_list_t;

size_t cx_list_length(const cx_list_t* list);
bool cx_list_is_contains(const cx_list_t* list, const cx_list_node_t* node);

CX_FORCE_INLINE void cx_list_reset(cx_list_t* list) {
	CX_ASSERT(list != NULL);
	cx_list_node_reset(&list->knot);
}

CX_FORCE_INLINE bool cx_list_is_empty(const cx_list_t* list) {
	CX_ASSERT(list != NULL);
	return !cx_list_node_is_linked(&list->knot);
}

CX_FORCE_INLINE cx_list_node_t* cx_list_knot(cx_list_t* list) {
	CX_ASSERT(list != NULL);
	return &list->knot;
}

CX_FORCE_INLINE cx_list_node_t* cx_list_first(cx_list_t* list) {
	CX_ASSERT(list != NULL);
	return list->knot.next;
}

CX_FORCE_INLINE cx_list_node_t* cx_list_last(cx_list_t* list) {
	CX_ASSERT(list != NULL);
	return list->knot.prev;
}

CX_FORCE_INLINE void cx_list_prepend(cx_list_t* list, cx_list_node_t* node) {
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);
	cx_list_node_link(node, cx_list_knot(list), cx_list_first(list));
}

CX_FORCE_INLINE void cx_list_append(cx_list_t* list, cx_list_node_t* node) {
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);
	cx_list_node_link(node, cx_list_last(list), cx_list_knot(list));
}

CX_FORCE_INLINE cx_list_node_t* cx_list_remove_first(cx_list_t* list) {
	cx_list_node_t* p;
	CX_ASSERT(list != NULL);
	p = cx_list_first(list);
	cx_list_node_unlink(p);
	return p;
}

CX_FORCE_INLINE cx_list_node_t* cx_list_remove_last(cx_list_t* list) {
	cx_list_node_t* p;
	CX_ASSERT(list != NULL);
	p = cx_list_last(list);
	cx_list_node_unlink(p);
	return p;
}

CX_FORCE_INLINE void cx_list_remove_node(cx_list_t* list, cx_list_node_t* node) {
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);
	CX_ASSERT(cx_list_is_contains(list, node));
	cx_list_node_unlink(node);
}

CX_END_DECLS

#define CX_LIST_FOREACH(pnode, plist) \
	for ((pnode) = cx_list_first(plist); (pnode) != cx_list_knot(list); (pnode) = cx_list_node_next(pnode))

#define CX_LIST_FOREACH_R(pnode, plist) \
	for ((pnode) = cx_list_last(plist); (pnode) != cx_list_knot(list); (pnode) = cx_list_node_prev(pnode))

#endif /* CX_LIST_H */
