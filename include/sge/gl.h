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

bool init(void);
void shutdown(void);
void draw_begin(void);
void draw_end(void);
SDL_Window *get_window(void);
Uint32 get_window_id(void);
SDL_GLContext get_context(void);

SGE_GL_END

#endif // SGE_GL_H
