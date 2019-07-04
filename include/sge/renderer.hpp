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
void set_view_matrix(const glm::mat4 &v);
void set_projection_matrix(const glm::mat4 &v);
bool begin(void);
void end(void);

static inline SDL_Window *window(void)
{
	return details::g_window;
}

static inline Uint32 window_id(void)
{
	return details::g_window_id;
}

static inline glm::ivec2 window_pos(void)
{
	return glm::ivec2(details::g_window_rect[0], details::g_window_rect[1]);
}

static inline glm::ivec2 window_size(void)
{
	return glm::ivec2(details::g_window_rect[2], details::g_window_rect[3]);
}

static inline glm::ivec4 window_rect(void)
{
	return glm::ivec4(details::g_window_rect[0], details::g_window_rect[1],
						details::g_window_rect[2], details::g_window_rect[3]);
}

static inline SDL_GLContext gl_context(void)
{
	return details::g_gl_context;
}

void center_next_imgui_window(const ImVec2& size);

SGE_RENDERER_END

#endif // SGE_RENDERER_HPP

