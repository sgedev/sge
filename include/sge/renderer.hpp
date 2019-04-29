//
//
#ifndef SGE_RENDERER_HPP
#define SGE_RENDERER_HPP

#include <imgui.h>

#include <sge/renderer/common.hpp>

SGE_RENDERER_BEGIN

bool init(void);
void shutdown(void);
bool handle_event(const SDL_Event &event);
SDL_Window *window(void);
Uint32 window_id(void);
glm::ivec2 window_pos(void);
glm::ivec2 window_size(void);
glm::ivec4 window_rect(void);
SDL_GLContext gl_context(void);
void set_view_matrix(const glm::mat4 &v);
void set_projection_matrix(const glm::mat4 &v);
bool begin(void);
void end(void);

SGE_RENDERER_END

#endif // SGE_RENDERER_HPP

