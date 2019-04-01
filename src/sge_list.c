/*
 *
 */
#include "sge_list.h"

int sge_list_get_length(sge_list_t *list)
{
	int n = 0;
	sge_list_node_t *p;

	SGE_LIST_FOREACH(p, list)
		n += 1;

	return n;
}

sge_bool_t sge_list_has(sge_list_t *list, sge_list_node_t *node)
{
	sge_list_node_t *p;

	SGE_LIST_FOREACH(node, list) {
		if (p == node)
			return SGE_TRUE;
	}

	return SGE_FALSE;
}

