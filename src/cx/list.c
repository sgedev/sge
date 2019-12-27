/*
 *
 */
#include <cx/list.h>

CX_API int cx_list_length(cx_list_t *list)
{
	int length = 0;
	cx_list_node_t *node;

	CX_ASSERT(list != NULL);

	CX_LIST_FOREACH(node, list) {
		length += 1;
	}

	return length;
}

CX_API int cx_list_is_contains(cx_list_t *list, cx_list_node_t *node)
{
	cx_list_node_t *temp;

	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);

	CX_LIST_FOREACH(temp, list) {
		if (temp == node)
			return 1;
	}

	return 0;
}
