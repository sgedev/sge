//
//
#include <GL/glew.h>

#include <sge/window.hpp>

SGE_WINDOW_BEGIN

enum {
	FLAG_VISIBLED = 0x1,
	FLAG_FULLSCREEN = 0x2,
};

static SDL_Window* s_window;
static glm::ivec4 s_rect;
static Uint32 s_id;
static SDL_GLContext s_gl_context;
static int s_flags;
static std::string s_title;

bool init(void)
{
	SGE_ASSERT(s_window == NULL);
	SGE_ASSERT(s_gl_context == NULL);

	SGE_LOGD("creating opengl window...\n");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

#ifdef SGE_DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	s_flags = 0;
	s_title = "SGE";

	s_window = SDL_CreateWindow(s_title.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (s_window == NULL)
		return false;

	s_id = SDL_GetWindowID(s_window);
	SDL_GetWindowPosition(s_window, &s_rect[0], &s_rect[1]);
	SDL_GetWindowSize(s_window, &s_rect[2], &s_rect[3]);

	SGE_LOGD("initializing opengl...\n");

	s_gl_context = SDL_GL_CreateContext(s_window);
	if (s_gl_context == NULL) {
		SGE_LOGE("failed to create opengl context.\n");
		SDL_DestroyWindow(s_window);
		s_window = NULL;
		return false;
	}

	SDL_GL_MakeCurrent(s_window, s_gl_context);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		SGE_LOGE("failed to initialize glew.\n");
		SDL_GL_DeleteContext(s_gl_context);
		s_gl_context = NULL;
		SDL_DestroyWindow(s_window);
		s_window = NULL;
		return false;
	}

	SGE_LOGI("opengl: %s\n", glGetString(GL_VERSION));
	SGE_LOGI("glew: %s\n", glewGetString(GLEW_VERSION));

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	return true;
}

void shutdown(void)
{
	SGE_ASSERT(s_window != NULL);
	SGE_ASSERT(s_gl_context != NULL);

	if (s_gl_context == SDL_GL_GetCurrentContext())
		SDL_GL_MakeCurrent(s_window, NULL);

	SDL_GL_DeleteContext(s_gl_context);
	s_gl_context = NULL;

	SDL_DestroyWindow(s_window);
	s_window = NULL;
}

void handle_event(const SDL_Event *event)
{
	SGE_ASSERT(event != NULL);

	if (event->window.windowID != s_id)
		return;

	switch (event->type) {
	case SDL_WINDOWEVENT:
		switch (event->window.event) {
		case SDL_WINDOWEVENT_MOVED:
			s_rect.x = event->window.data1;
			s_rect.y = event->window.data2;
			break;
		case SDL_WINDOWEVENT_RESIZED:
			s_rect.z = event->window.data1;
			s_rect.w = event->window.data2;
			break;
		case SDL_WINDOWEVENT_EXPOSED:
		case SDL_WINDOWEVENT_SHOWN:
			s_flags |= FLAG_VISIBLED;
			break;
		case SDL_WINDOWEVENT_HIDDEN:
		case SDL_WINDOWEVENT_MINIMIZED:
			s_flags &= ~FLAG_VISIBLED;
			break;
		}
		break;
	case SDL_KEYDOWN:
		if (event->key.keysym.sym == SDLK_RETURN && event->key.keysym.mod & KMOD_ALT) {
			if (s_flags & FLAG_FULLSCREEN) {
				s_flags &= ~FLAG_FULLSCREEN;
				SDL_SetWindowFullscreen(s_window, 0);
			} else {
				s_flags |= FLAG_FULLSCREEN;
				SDL_SetWindowFullscreen(s_window, SDL_WINDOW_FULLSCREEN);
			}
		}
		break;
	}
}

bool draw_begin(void)
{
	SGE_ASSERT(s_window != NULL);
	SGE_ASSERT(s_gl_context != NULL);

	if (!visibled())
		return false;

	if (s_rect[2] < 1 || s_rect[3] < 1)
		return false;

	if (SDL_GL_MakeCurrent(s_window, s_gl_context))
		return false;

	glViewport(0, 0, s_rect[2], s_rect[3]);
	glClear(GL_COLOR_BUFFER_BIT);

	return true;
}

void draw_end(void)
{
	SGE_ASSERT(s_window != NULL);
	SDL_GL_SwapWindow(s_window);
}

SDL_Window* to_sdl_window(void)
{
	SGE_ASSERT(s_window != NULL);
	return s_window;
}

SDL_GLContext sdl_gl_context(void)
{
	SGE_ASSERT(s_window != NULL);
	return s_gl_context;
}

Uint32 sdl_id(void)
{
	SGE_ASSERT(s_window != NULL);
	return s_id;
}

const glm::ivec4 &rect(void)
{
	SGE_ASSERT(s_window != NULL);
	return s_rect;
}

bool visibled(void)
{
	SGE_ASSERT(s_window != NULL);
	return (s_flags & FLAG_VISIBLED);
}

bool fullscreen(void)
{
	SGE_ASSERT(s_window != NULL);
	return (s_flags & FLAG_FULLSCREEN);
}

const char *title(void)
{
	SGE_ASSERT(s_window != NULL);
	return s_title.c_str();
}

void set_title(const char* title)
{
	SGE_ASSERT(s_window != NULL);
	s_title = title;
	SDL_SetWindowTitle(s_window, title);
}

SGE_WINDOW_END
