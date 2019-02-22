//
//
#ifndef SGE_GL_HPP
#define SGE_GL_HPP

#include <sge/gl/common.hpp>
#include <sge/gl/buffer.hpp>
#include <sge/gl/shader.hpp>
#include <sge/gl/program.hpp>
#include <sge/gl/vertex_array.hpp>

SGE_GL_BEGIN

bool init(void);
void shutdown(void);
void draw_begin(void);
void draw_end(void);
SDL_Window *get_window(void);
Uint32 get_window_id(void);
SDL_GLContext get_context(void);

SGE_GL_END

#endif // SGE_GL_HPP
