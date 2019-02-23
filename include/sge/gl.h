//
//
#ifndef SGE_GL_H
#define SGE_GL_H

#include <sge/gl/common.h>
#include <sge/gl/buffer.h>
#include <sge/gl/shader.h>
#include <sge/gl/program.h>
#include <sge/gl/vertex_array.h>

SGE_GL_BEGIN

bool Init(void);
void Shutdown(void);
void DrawBegin(void);
void DrawEnd(void);
SDL_Window *GetWindow(void);
Uint32 GetWindowID(void);
SDL_GLContext GetContext(void);

SGE_GL_END

#endif // SGE_GL_H
