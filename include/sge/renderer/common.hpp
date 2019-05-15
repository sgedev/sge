//
//
#ifndef SGE_RENDERER_COMMON_HPP
#define SGE_RENDERER_COMMON_HPP

#include <GL/gl.hpp>

#include <sge/common.hpp>

#define SGE_RENDERER_BEGIN SGE_BEGIN namespace renderer {
#define SGE_RENDERER_END } SGE_END

SGE_RENDERER_BEGIN

namespace details {
	extern SDL_Window *g_window;
	extern Uint32 g_window_id;
	extern int g_window_rect[4];
	extern bool g_window_visibled;
	extern SDL_GLContext g_gl_context;
};

SGE_RENDERER_END

#endif // SGE_RENDERER_COMMON_HPP

