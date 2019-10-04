/*
 *
 */
#ifndef CX_LIST_H
#define CX_LIST_H

#include <cx/common.h>

CX_BEGIN_DECLS

typedef struct CXListNodeStruct {
	struct CXListNodeStruct *prev;
	struct CXListNodeStruct *next;
} CXListNode;

static inline void CXResetListNode(CXListNode *node)
{
	CX_ASSERT(node != NULL);

	node->prev = node;
	node->next = node;
}

static inline void CXLinkListNode(
	CXListNode *node, CXListNode *prev, CXListNode *next)
{
	CX_ASSERT(node != NULL);
	CX_ASSERT(prev != NULL);
	CX_ASSERT(next != NULL);

	node->prev = prev;
	node->next = next;
	prev->next = node;
	next->prev = node;
}

static inline void CXUnlinkListNode(CXListNode *node)
{
	CX_ASSERT(node != NULL);
	CX_ASSERT(node->prev != NULL);
	CX_ASSERT(node->next != NULL);

	node->prev->next = node->next;
	node->next->prev = node->prev;

	CXResetListNode(node);
}

static inline int CXIsListNodeLinked(const CXListNode *node)
{
	CX_ASSERT(node != NULL);

	return (node != node->next);
}

static inline CXListNode *CXGetPrevListNode(CXListNode *node)
{
	CX_ASSERT(node != NULL);
	CX_ASSERT(node->prev != NULL);

	return node->prev;
}

static inline CXListNode *CXGetNextListNode(CXListNode *node)
{
	CX_ASSERT(node != NULL);
	CX_ASSERT(node->next != NULL);

	return node->next;
}

typedef struct {
	CXListNode knot;
} CXList;

CX_API int CXGetListLength(CXList *list);
CX_API int CXIsListContains(CXList *list, CXListNode *node);

static inline void CXResetList(CXList *list)
{
	CX_ASSERT(list != NULL);

	CXResetListNode(&list->knot);
}

static inline int CXIsListEmpty(const CXList *list)
{
	CX_ASSERT(list != NULL);

	return !CXIsListNodeLinked(&list->knot);
}

static inline CXListNode *CXGetListKnot(CXList *list)
{
	CX_ASSERT(list != NULL);

	return &list->knot;
}

static inline CXListNode *CXGetListHead(CXList *list)
{
	CX_ASSERT(list != NULL);

	return CXGetNextListNode(&list->knot);
}

static inline CXListNode *CXGetListTail(CXList *list)
{
	CX_ASSERT(list != NULL);

	return CXGetPrevListNode(&list->knot);
}

static inline CXListNode *CXDeleteListHead(CXList *list)
{
	CXListNode *node;

	CX_ASSERT(list != NULL);

	node = CXGetListHead(list);
	CXUnlinkListNode(node);

	return node;
}

static inline CXListNode *CXDeleteListTail(CXList *list)
{
	CXListNode *node;

	CX_ASSERT(list != NULL);

	node = CXGetListTail(list);
	CXUnlinkListNode(node);

	return node;
}

static inline void CXDeleteListNode(CXList *list, CXListNode *node)
{
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);
	CX_ASSERT(CXIsListContains(list, node));

	CXUnlinkListNode(node);
}

static inline void CXClearList(CXList *list)
{
	CX_ASSERT(list != NULL);

	while (!CXIsListEmpty(list))
		CXDeleteListHead(list);
}

static inline void CXAddListHead(CXList *list, CXListNode *node)
{
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);

	CXUnlinkListNode(node);
	CXLinkListNode(node, CXGetListKnot(list), CXGetListHead(list));	
}

static inline void CXAddListTail(CXList *list, CXListNode *node)
{
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);

	CXUnlinkListNode(node);
	CXLinkListNode(node, CXGetListTail(list), CXGetListKnot(list));	
}

CX_END_DECLS

#define CX_LIST_FOREACH(pnode, plist) \
	for ((pnode)  = CXGetListHead(plist); \
		 (pnode) != CXGetListKnot(plist); \
		 (pnode)  = CXGetNextListNode(pnode))

#endif /* CX_LIST_H */

