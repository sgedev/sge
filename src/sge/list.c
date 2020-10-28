/*
 *
 */
#include <sge/list.h>

int sge_list_length(sge_list_t *list)
{
    int length = 0;
    sge_list_node_t *node;

    SGE_ASSERT(list != NULL);

    SGE_LIST_FOREACH(node, list) {
        length += 1;
    }

    return length;
}

bool sge_list_contains(sge_list_t *list, sge_list_node_t *node)
{
    sge_list_node_t *temp;

    SGE_ASSERT(list != NULL);
    SGE_ASSERT(node != NULL);

    SGE_LIST_FOREACH(temp, list) {
        if (temp == node)
            return true;
    }

    return false;
}
