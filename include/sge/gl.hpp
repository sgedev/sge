//
//
#ifndef SGL_GL_H
#define SGL_GL_H

#include "SGL_gl_common.h"
#include "SGL_gl_buffer.h"
#include "SGL_gl_shader.h"
#include "SGL_gl_program.h"
#include "SGL_gl_vertex_array.h"

SGL_GL_BEGIN

bool Init(void);
void Shutdown(void);
void DrawBegin(void);
void DrawEnd(void);
SDL_Window *GetWindow(void);
Uint32 GetWindowID(void);
SDL_GLContext GetContext(void);

SGL_GL_END

#endif // SGL_GL_H
