//
//
#include <GL/glew.h>

#include <sge.hpp>

SGE_BEGIN

enum {
	WINDOW_VISIBLE = 0x1
};

static SDL_Window *window;
static Uint32 window_id;
static int window_size[2];
static int window_flags;
static SDL_GLContext gl_context;

static bool create_window(void)
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

    SGE_ASSERT(window == nullptr);
    window = SDL_CreateWindow("sge",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == nullptr) {
        SGE_LOGE("failed to create main window.");
		goto bad0;
	}

    window_flags = WINDOW_VISIBLE;
    window_id = SDL_GetWindowID(window);
    SDL_GetWindowSize(window, &window_size[0], &window_size[1]);

    SGE_ASSERT(gl_context == nullptr);
    gl_context = SDL_GL_CreateContext(window);
    if (gl_context == nullptr) {
        SGE_LOGE("failed to create main gl context.");
		goto bad1;
	}

    if (SDL_GL_MakeCurrent(window, gl_context) < 0) {
        SGE_LOGE("failed to make current gl context.");
		goto bad2;
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
        SGE_LOGE("failed to init GLEW.");
		goto bad2;
	}

	if (!renderer::init()) {
        SGE_LOGE("failed to init renderer.");
		goto bad2;
	}

	return true;

bad2:
    SDL_GL_DeleteContext(gl_context);
    gl_context = nullptr;

bad1:
    SDL_DestroyWindow(window);
    window = nullptr;

bad0:
	return false;
}

static void destroy_window(void)
{
	renderer::shutdown();

    SGE_ASSERT(gl_context != nullptr);
    SDL_GL_DeleteContext(gl_context);
    gl_context = nullptr;

    SGE_ASSERT(window != nullptr);
    SDL_DestroyWindow(window);
    window = nullptr;
}

static void handle_window_event(const SDL_WindowEvent &evt)
{
	switch (evt.event) {
	case SDL_WINDOWEVENT_SHOWN:
        window_flags |= WINDOW_VISIBLE;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
        window_flags &= ~WINDOW_VISIBLE;
		break;
	case SDL_WINDOWEVENT_RESIZED:
        window_size[0] = evt.data1;
        window_size[1] = evt.data2;
		break;
	}
}

static void update_window(void)
{
    SGE_ASSERT(window != nullptr);
    SGE_ASSERT(gl_context != nullptr);

    if (!(window_flags & WINDOW_VISIBLE))
		return;

    if (window_size[0] < 1 || window_size[1] < 1)
		return;

    if (SDL_GL_MakeCurrent(window, gl_context) < 0)
		return;

    renderer::default_target.resize(window_size[0], window_size[1]);

    renderer::begin();

    // TODO

    renderer::end();

    SDL_GL_SwapWindow(window);
}

static void vm_handler(vm::output &r)
{
    // TODO update graphics
    // TODO update sound
}

bool start(uv_loop_t *loop, const std::string &root, const std::string &init)
{
    if (!create_window())
        return false;

    if (!sound::init()) {
        destroy_window();
        return false;
    }

    return vm::start(loop, root, init, &vm_handler);
}

void stop(void)
{
	vm::stop();
    sound::shutdown();
    destroy_window();
}

void post_event(const SDL_Event &evt)
{
	if (evt.type != SDL_WINDOWEVENT) {
		vm::post_event(evt);
		return;
	}

    if (window != nullptr && evt.window.windowID == window_id)
        handle_window_event(evt.window);
}

SGE_END
