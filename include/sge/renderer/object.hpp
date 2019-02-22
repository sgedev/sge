//
//
#ifndef SGL_RENDERER_OBJECT_H
#define SGL_RENDERER_OBJECT_H

#include "SGL_renderer_common.h"

SGL_RENDERER_BEGIN

class Object {
public:
	Object(const SGL_ObjectType *type);
	virtual ~Object(void);
};

SGL_RENDERER_END

#endif // SGL_RENDERER_OBJECT_H
