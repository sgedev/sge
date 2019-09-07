/*
 *
 */
#include <sge/list.h>

int sge_list_length(sge_list *list)
{
	int length = 0;
	sge_list_node *node;

	SGE_ASSERT(list != NULL);

	SGE_LIST_FOREACH(node, list) {
		length += 1;
	}

	return length;
}

int sge_list_has(sge_list *list, sge_list_node *node)
{
	sge_list_node *temp;

	SGE_ASSERT(list != NULL);
	SGE_ASSERT(node != NULL);

	SGE_LIST_FOREACH(temp, list) {
		if (temp == node)
			return 1;
	}

	return 0;
}

