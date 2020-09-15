//
//
#include <GL/glew.h>

#include <sge.hpp>

SGE_BEGIN

enum {
	WINDOW_VISIBLE = 0x1
};

static SDL_Window *main_window;
static Uint32 main_window_id;
static int main_window_rect[4];
static int main_window_flags;
static SDL_GLContext main_gl_context;

static bool init_main_window(void)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	
#ifdef SGE_DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	SGE_ASSERT(main_window == nullptr);
	main_window = SDL_CreateWindow("sge",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
	if (main_window == nullptr) {
		SGE_LOGE("Failed to create main window.");
		goto bad0;
	}

	main_window_flags = WINDOW_VISIBLE;
	main_window_id = SDL_GetWindowID(main_window);
	SDL_GetWindowPosition(main_window, &main_window_rect[0], &main_window_rect[1]);
	SDL_GetWindowSize(main_window, &main_window_rect[2], &main_window_rect[3]);

	SGE_ASSERT(main_gl_context == nullptr);
	main_gl_context = SDL_GL_CreateContext(main_window);
	if (main_gl_context == nullptr) {
		SGE_LOGE("Failed to create main gl context.");
		goto bad1;
	}

	if (SDL_GL_MakeCurrent(main_window, main_gl_context) < 0) {
		SGE_LOGE("Failed to make current gl context.");
		goto bad2;
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SGE_LOGE("Failed to init GLEW.");
		goto bad2;
	}

	if (!renderer::init()) {
		SGE_LOGE("Failed to init renderer.");
		goto bad2;
	}

	return true;

bad2:
	SDL_GL_DeleteContext(main_gl_context);
	main_gl_context = nullptr;

bad1:
	SDL_DestroyWindow(main_window);
	main_window = nullptr;

bad0:
	return false;
}

static void shutdown_main_window(void)
{
	renderer::shutdown();

	SGE_ASSERT(main_gl_context != nullptr);
	SDL_GL_DeleteContext(main_gl_context);
	main_gl_context = nullptr;

	SGE_ASSERT(main_window != nullptr);
	SDL_DestroyWindow(main_window);
	main_window = nullptr;
}

static void handle_main_window_event(const SDL_WindowEvent &evt)
{
	switch (evt.event) {
	case SDL_WINDOWEVENT_SHOWN:
		main_window_flags |= WINDOW_VISIBLE;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		main_window_flags &= ~WINDOW_VISIBLE;
		break;
	case SDL_WINDOWEVENT_MOVED:
		main_window_rect[0] = evt.data1;
		main_window_rect[1] = evt.data2;
		break;
	case SDL_WINDOWEVENT_RESIZED:
		main_window_rect[2] = evt.data1;
		main_window_rect[3] = evt.data2;
		break;
	}
}

static void render(void)
{
	SGE_ASSERT(main_window != nullptr);
	SGE_ASSERT(main_gl_context != nullptr);

	if (!(main_window_flags & WINDOW_VISIBLE))
		return;

	if (main_window_rect[2] < 1 || main_window_rect[3] < 1)
		return;

	if (SDL_GL_MakeCurrent(main_window, main_gl_context) < 0)
		return;

	glViewport(0, 0, main_window_rect[2], main_window_rect[3]);

	glClear(GL_COLOR_BUFFER_BIT);

	SDL_GL_SwapWindow(main_window);
}

static bool init(void)
{
	if (!init_main_window())
		return false;

	return true;
}

static void shutdown(void)
{
	shutdown_main_window();
}

bool start(uv_loop_t *loop)
{
    return vm::start(loop, nullptr);
}

void stop(void)
{
	vm::stop();
}

void post_event(const SDL_Event &evt)
{
	if (evt.type != SDL_WINDOWEVENT) {
		vm::post_event(evt);
		return;
	}

	if (main_window != nullptr && evt.window.windowID == main_window_id)
		handle_main_window_event(evt.window);
}

SGE_END
