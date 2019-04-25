//
//
#ifndef SGE_GL_HPP
#define SGE_GL_HPP

#include <sge/gl/common.hpp>
#include <sge/gl/window.hpp>
#include <sge/gl/context.hpp>

SGE_GL_BEGIN

bool init(void);
void shutdown(void);
bool make_current(void);
void swap_buffers(void);
void handle_event(const SDL_Event &event);
SDL_Window *window(void);
Uint32 window_id(void);
const glm::ivec2 &window_pos(void);
const glm::ivec2 &window_size(void);
SDL_GLContext context(void);

SGE_GL_END

#endif // SGE_GL_HPP

