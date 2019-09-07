//
//
#ifndef SGE_WINDOW_HPP
#define SGE_WINDOW_HPP

#include <string>

#include <sge/common.hpp>

#define SGE_WINDOW_BEGIN SGE_BEGIN namespace window {
#define SGE_WINDOW_END } SGE_END

SGE_WINDOW_BEGIN

bool init(void);
void shutdown(void);
void handle_event(const SDL_Event *event);
bool draw_begin(void);
void draw_end(void);
SDL_Window *to_sdl_window(void);
SDL_GLContext sdl_gl_context(void);
Uint32 sdl_id(void);
const glm::ivec4 &rect(void);
bool visibled(void);
bool fullscreen(void);
const char *title(void);
void set_title(const char *title);

SGE_WINDOW_END

#endif // SGE_WINDOW_HPP

