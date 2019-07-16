//
//
#ifndef SGE_GL_HPP
#define SGE_GL_HPP

#include <sge/gl/common.hpp>
#include <sge/gl/buffer.hpp>
#include <sge/gl/program.hpp>
#include <sge/gl/shader.hpp>
#include <sge/gl/vertex_array.hpp>

SGE_GL_BEGIN

bool init(void);
void shutdown(void);
bool handle_event(const SDL_Event &event);
bool make_current(void);
void swap_buffers(void);
SDL_Window *window(void);
SDL_GLContext context(void);

SGE_GL_END

#endif // SGE_GL_HPP

