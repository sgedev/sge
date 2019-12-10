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

static inline void cxResetListNode(CXListNode *node)
{
	CX_ASSERT(node != NULL);

	node->prev = node;
	node->next = node;
}

static inline void cxLinkListNode(
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

static inline void cxUnlinkListNode(CXListNode *node)
{
	CX_ASSERT(node != NULL);
	CX_ASSERT(node->prev != NULL);
	CX_ASSERT(node->next != NULL);

	node->prev->next = node->next;
	node->next->prev = node->prev;

	cxResetListNode(node);
}

static inline int cxIsListNodeLinked(const CXListNode *node)
{
	CX_ASSERT(node != NULL);

	return (node != node->next);
}

static inline CXListNode *cxGetPrevListNode(CXListNode *node)
{
	CX_ASSERT(node != NULL);
	CX_ASSERT(node->prev != NULL);

	return node->prev;
}

static inline CXListNode *cxGetNextListNode(CXListNode *node)
{
	CX_ASSERT(node != NULL);
	CX_ASSERT(node->next != NULL);

	return node->next;
}

typedef struct {
	CXListNode knot;
} CXList;

CX_API int cxGetListLength(CXList *list);
CX_API int cxIsListContains(CXList *list, CXListNode *node);

static inline void CXResetList(CXList *list)
{
	CX_ASSERT(list != NULL);

	cxResetListNode(&list->knot);
}

static inline int cxIsListEmpty(const CXList *list)
{
	CX_ASSERT(list != NULL);

	return !cxIsListNodeLinked(&list->knot);
}

static inline CXListNode *cxGetListKnot(CXList *list)
{
	CX_ASSERT(list != NULL);

	return &list->knot;
}

static inline CXListNode *cxGetListHead(CXList *list)
{
	CX_ASSERT(list != NULL);

	return cxGetNextListNode(&list->knot);
}

static inline CXListNode *cxGetListTail(CXList *list)
{
	CX_ASSERT(list != NULL);

	return cxGetPrevListNode(&list->knot);
}

static inline CXListNode *cxDeleteListHead(CXList *list)
{
	CXListNode *node;

	CX_ASSERT(list != NULL);

	node = cxGetListHead(list);
	cxUnlinkListNode(node);

	return node;
}

static inline CXListNode *cxDeleteListTail(CXList *list)
{
	CXListNode *node;

	CX_ASSERT(list != NULL);

	node = cxGetListTail(list);
	cxUnlinkListNode(node);

	return node;
}

static inline void cxDeleteListNode(CXList *list, CXListNode *node)
{
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);
	CX_ASSERT(cxIsListContains(list, node));

	cxUnlinkListNode(node);
}

static inline void cxClearList(CXList *list)
{
	CX_ASSERT(list != NULL);

	while (!cxIsListEmpty(list))
		cxDeleteListHead(list);
}

static inline void cxAddListHead(CXList *list, CXListNode *node)
{
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);

	cxUnlinkListNode(node);
	cxLinkListNode(node, cxGetListKnot(list), cxGetListHead(list));	
}

static inline void cxAddListTail(CXList *list, CXListNode *node)
{
	CX_ASSERT(list != NULL);
	CX_ASSERT(node != NULL);

	cxUnlinkListNode(node);
	cxLinkListNode(node, cxGetListTail(list), cxGetListKnot(list));	
}

CX_END_DECLS

#define CX_LIST_FOREACH(pnode, plist) \
	for ((pnode)  = cxGetListHead(plist); \
		 (pnode) != cxGetListKnot(plist); \
		 (pnode)  = cxGetNextListNode(pnode))

#endif /* CX_LIST_H */

