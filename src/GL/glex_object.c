/*
 *
 */
#include "glex_object.h"

#define GLEX_OBJECT_MAGIC 0x20120920

typedef struct {
	unsigned int magic;
	const GLEXType *type;
	int ref;
} GLEXObjectHead;

static inline void *glexObjectFromHead(GLEXObjectHead *head)
{
	GLEX_ASSERT(head != NULL);
	return CX_PMOVB(head, sizeof(GLEXObjectHead));
}

static inline GLEXObjectHead *glexObjectHead(void *obj)
{
	GLEX_ASSERT(obj != NULL);
	return (GLEXObjectHead *)CX_PMOVB(obj, -sizeof(GLEXObjectHead));
}

void *glexNew(const GLEXType *type)
{
	GLEXObjectHead *head;
	void *obj;

	GLEX_ASSERT(type != NULL);

	head = glexAllocMemory(sizeof(GLEXObjectHead) + type->size);
	if (head == NULL)
		return NULL;

	head->magic = GLEX_OBJECT_MAGIC;
	head->type = type;
	head->ref = 1;

	obj = glexObjectFromHead(head);
	if (type->init(obj) < 0) {
		glexFreeMemory(head);
		return NULL;
	}

	return obj;
}

void *glexRef(void *obj)
{
	GLEXObjectHead *head;

	GLEX_ASSERT(obj != NULL);
	head = glexObjectHead(obj);
	GLEX_ASSERT(head->magic == GLEX_OBJECT_MAGIC);
	GLEX_ASSERT(head->ref > 0);

	head->ref += 1;

	return obj;
}

void glexUnref(void *obj)
{
	GLEXObjectHead *head;

	GLEX_ASSERT(obj != NULL);
	head = glexObjectHead(obj);
	GLEX_ASSERT(head->magic == GLEX_OBJECT_MAGIC);
	GLEX_ASSERT(head->ref > 0);

	if (head->ref == 1) {
		head->type->fina(obj);
		glexFreeMemory(head);
	} else
		head->ref -= 1;
}

const GLEXType *glexGetObjectType(void *obj)
{
	GLEXObjectHead *head;

	GLEX_ASSERT(obj != NULL);
	head = glexObjectHead(obj);
	GLEX_ASSERT(head->magic == GLEX_OBJECT_MAGIC);
	GLEX_ASSERT(head->ref > 0);

	return head->type;
}

