/*
 *
 */
#include <cx/list.h>

size_t cx_list_length(const cx_list_t* list) {
	size_t length = 0;
	const cx_list_node_t* p;

	CX_ASSERT(list != NULL);

	for (p = list->knot.next; p != &list->knot; p = p->next) {
		length += 1;
	}

	return length;
}

bool cx_list_is_contains(const cx_list_t* list, const cx_list_node_t* node) {
	const cx_list_node_t* p;

	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);

	for (p = list->knot.next; p != &list->knot; p = p->next) {
		if (node == p) {
			return true;
		}
	}

	return false;
}
