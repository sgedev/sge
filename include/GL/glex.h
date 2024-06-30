/*
 *
 */
#ifndef GLEX_H
#define GLEX_H

#include <stdbool.h>

#include <GL/glew.h>
#include <cx/common.h>

CX_BEGIN_DECLS

typedef struct GLEXContextStruct GLEXContext;

bool glexInit();
void glexShutdown();

GLEXContext* glexCreateContext();
GLEXContext* glexCurrentContext();
void glexDeleteContext(GLEXContext* context);
void glexMakeCurrent(GLEXCotnext* context);

CX_END_DECLS

#endif /* GLEX_H */
