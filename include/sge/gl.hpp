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
SDL_Window *get_window(void);
SDL_GLContext get_context(void);
void handle_event(const SDL_Event &event);
bool begin(void);
void end(void);

SGE_GL_END

#endif // SGE_GL_HPP

