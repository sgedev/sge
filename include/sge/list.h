//
//
#ifndef SGE_LIST_H
#define SGE_LIST_H

#include <sge/common.h>

SGE_C_BEGIN

typedef struct sge_ListNodeStruct {
    struct sge_ListNodeStruct* prev;
    struct sge_ListNodeStruct* next;
} sge_ListNode;

typedef struct {
    sge_ListNode knot;
} sge_List;

SGE_INLINE static void sge_ListNode_reset(sge_ListNode* node) {
    SGE_ASSERT(node != NULL);
    node->prev = node;
    node->next = node;
}

SGE_INLINE static bool sge_ListNode_isLinked(const sge_ListNode* node) {
    SGE_ASSERT(node != NULL);
    return (node->next != node);
}

SGE_INLINE static void sge_ListNode_link(sge_ListNode* node, sge_ListNode* prev, sge_ListNode* next) {
    SGE_ASSERT(node != NULL);
    SGE_ASSERT(prev != NULL);
    SGE_ASSERT(next != NULL);
    node->prev = prev;
    node->next = next;
    prev->next = node;
    next->prev = node;
}

SGE_INLINE static void sge_ListNode_unlink(sge_ListNode* node) {
    SGE_ASSERT(node != NULL);
    SGE_ASSERT(node->prev != NULL);
    SGE_ASSERT(node->next != NULL);
    node->prev->next = node->next;
    node->next->prev = node->prev;
    sge_ListNode_reset(node);
}

SGE_INLINE static sge_ListNode* sge_ListNode_prev(sge_ListNode* node) {
    SGE_ASSERT(node != NULL);
    return node->prev;
}

SGE_INLINE static const sge_ListNode* sge_ListNode_prev_const(const sge_ListNode* node) {
    SGE_ASSERT(node != NULL);
    return node->prev;
}

SGE_INLINE static sge_ListNode* sge_ListNode_next(sge_ListNode* node) {
    SGE_ASSERT(node != NULL);
    return node->next;
}

SGE_INLINE static const sge_ListNode* sge_ListNode_next_const(const sge_ListNode* node) {
    SGE_ASSERT(node != NULL);
    return node->next;
}

SGE_INLINE static void sge_List_reset(sge_List* list) {
    SGE_ASSERT(list != NULL);
    sge_ListNode_reset(&list->knot);
}

SGE_INLINE static sge_ListNode* sge_List_knot(sge_List* list) {
    SGE_ASSERT(list != NULL);
    return &list->knot;
}

SGE_INLINE static const sge_ListNode* sge_List_knot_const(const sge_List* list) {
    SGE_ASSERT(list != NULL);
    return &list->knot;
}

SGE_INLINE static sge_ListNode* sge_List_first(sge_List* list) {
    SGE_ASSERT(list != NULL);
    return sge_ListNode_next(&list->knot);
}

SGE_INLINE static const sge_ListNode* sge_List_first_const(const sge_List* list) {
    SGE_ASSERT(list != NULL);
    return sge_ListNode_next_const(&list->knot);
}

SGE_INLINE static sge_ListNode* sge_List_last(sge_List* list) {
    SGE_ASSERT(list != NULL);
    return sge_ListNode_prev(&list->knot);
}

SGE_INLINE static const sge_ListNode* sge_List_last_const(const sge_List* list) {
    SGE_ASSERT(list != NULL);
    return sge_ListNode_prev_const(&list->knot);
}

SGE_INLINE static bool sge_List_isEmpty(const sge_List* list) {
    SGE_ASSERT(list != NULL);
    return !sge_ListNode_isLinked(&list->knot);
}

SGE_INLINE static size_t sge_List_length(const sge_List* list) {
    SGE_ASSERT(list != NULL);
    size_t n = 0;
    for (const sge_ListNode* node = sge_List_first_const(list); node != sge_List_knot_const(list); node = sge_ListNode_next_const(node)) {
        n += 1;
    }
    return n;
}

SGE_INLINE static bool sge_List_contains(const sge_List* list, const sge_ListNode* node) {
    SGE_ASSERT(list != NULL);
    SGE_ASSERT(node != NULL);
    for (const sge_ListNode* p = sge_List_first_const(list); p != sge_List_knot_const(list); p = sge_ListNode_next_const(node)) {
        if (p == node) {
            return true;
        }
    }
    return false;
}

SGE_INLINE static void sge_List_prepend(sge_List* list, sge_ListNode* node) {
    SGE_ASSERT(list != NULL);
    SGE_ASSERT(node != NULL);
    sge_ListNode_link(node, sge_List_knot(list), sge_List_first(list));
}

SGE_INLINE static void sge_List_append(sge_List* list, sge_ListNode* node) {
    SGE_ASSERT(list != NULL);
    SGE_ASSERT(node != NULL);
    sge_ListNode_link(node, sge_List_last(list), sge_List_knot(list));
}

SGE_INLINE static sge_ListNode* sge_List_removeFirst(sge_List* list) {
    SGE_ASSERT(list != NULL);
    sge_ListNode* node = sge_List_first(list);
    sge_ListNode_unlink(node);
    return node;
}

SGE_INLINE static sge_ListNode* sge_List_removeLast(sge_List* list) {
    SGE_ASSERT(list != NULL);
    sge_ListNode* node = sge_List_last(list);
    sge_ListNode_unlink(node);
    return node;
}

SGE_INLINE static void sge_List_removeNode(sge_List* list, sge_ListNode* node) {
    SGE_ASSERT(list != NULL);
    SGE_ASSERT(node != NULL);
    SGE_ASSERT(sge_List_contains(list, node));
    sge_ListNode_unlink(node);
}

SGE_C_END

#define SGE_LIST_FOREACH(pnode, plist) \
    for ((pnode)  = sge_List_first(plist); \
         (pnode) != sge_List_knot(plist); \
         (pnode)  = sge_ListNode_next(pnode))

#define SGE_LIST_FOREACH_R(pnode, plist) \
    for ((pnode)  = sge_List_last(plist); \
         (pnode) != sge_List_knot(plist); \
         (pnode)  = sge_ListNode_prev(pnode))

#endif // SGE_LIST_H
