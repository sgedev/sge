/*
 *
 */
#include <cx/list.h>

CX_API int CXGetListLength(CXList *list)
{
	int length = 0;
	CXListNode *node;

	CX_ASSERT(list != NULL);

	CX_LIST_FOREACH(node, list) {
		length += 1;
	}

	return length;
}

int CXIsListContains(CXList *list, CXListNode *node)
{
	CXListNode *temp;

	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);

	CX_LIST_FOREACH(temp, list) {
		if (temp == node)
			return 1;
	}

	return 0;
}

