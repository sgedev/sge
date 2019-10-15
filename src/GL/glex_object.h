/*
 *
 */
#ifndef GLEX_OBJECT_H
#define GLEX_OBJECT_H

#include "glex_common.h"

typedef struct {
	const char *name;
	unsigned int size;
	int (*init)(void *obj);
	void (*fina)(void *obj);
} GLEXType;

void *glexNew(const GLEXType *type);
void *glexRef(void *obj);
void glexUnref(void *obj);
const GLEXType *glexGetObjectType(void *obj);

#define GLEX_OBJECT_IS_A(obj, type) \
	(glexGetObjectType(obj) == (type))

#endif /* GLEX_OBJECT_H */

