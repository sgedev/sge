//
//
#include <iostream>

#include "SGL_gl.h"
#include "SGL_game.h"

SGL_GL_BEGIN

static SDL_Window *Window;
static SDL_GLContext Context;
static Uint32 WindowID;

static const char *DebugSource(GLenum source)
{
	switch (source) {
	case GL_DEBUG_SOURCE_API:             return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "WINDOW_SYSTEM";
	case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER_COMPILER";
	case GL_DEBUG_SOURCE_THIRD_PARTY:     return "THIRD_PART";
	case GL_DEBUG_SOURCE_APPLICATION:     return "APPLICATION";
	case GL_DEBUG_SOURCE_OTHER:           return "OTHER";
	}
	return "";
}

static const char *DebugType(GLenum type)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:               return "ERROR";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "UNDEFINED_BEHAVIOR";
	case GL_DEBUG_TYPE_PORTABILITY:         return "PORTABILITY";
	case GL_DEBUG_TYPE_PERFORMANCE:         return "PERFORMANCE";
	case GL_DEBUG_TYPE_MARKER:              return "MARKER";
	case GL_DEBUG_TYPE_PUSH_GROUP:          return "PUSH_GROUP";
	case GL_DEBUG_TYPE_POP_GROUP:           return "POP_GROUP";
	case GL_DEBUG_TYPE_OTHER:               return "OTHER";
	}
	return "";
}

static const char *DebugSeverity(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:         return "HIGH";
	case GL_DEBUG_SEVERITY_MEDIUM:       return "MEDIUM";
	case GL_DEBUG_SEVERITY_LOW:          return "LOW";
	case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
	}
	return "";
}

static void DebugOutput(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar *message, const void *data)
{
	SGL_LOGD("[GL-%04d|%s|%s|%s] %s", id,
		DebugSource(source), DebugType(type), DebugSeverity(severity), message);
}

bool Init(void)
{
	SGL_LOGD("Creating OpenGL window...\n");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	Eigen::Vector4i rect = Game::Window::GetRect();

	Window = SDL_CreateWindow(Game::GetName(),
		rect[0], rect[1], rect[2], rect[3],
		Game::Window::GetFlags() | SDL_WINDOW_OPENGL);

	if (Window == NULL)
		return false;

	SGL_LOGD("Initializing OpenGL...\n");

	Context = SDL_GL_CreateContext(Window);
	if (Context == NULL) {
		SDL_DestroyWindow(Window);
		SGL_LOGE("Failed to create OpenGL context.\n");
		return false;
	}

	SDL_GL_MakeCurrent(Window, Context);

	SGL_LOGD("Initializing GLEW...\n");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SDL_GL_DeleteContext(Context);
		SDL_DestroyWindow(Window);
		SGL_LOGE("Failed to initialize GLEW.\n");
		return false;
	}

#ifdef SGL_DEBUG
	if (GL_KHR_debug) {
		SGL_LOGD("Enable OpenGL debug output.\n");
		glDebugMessageCallback(DebugOutput, NULL);
	}
#endif

	WindowID = SDL_GetWindowID(Window);

	return true;
}

void Shutdown(void)
{
	SGL_ASSERT(Window != NULL);
	SGL_ASSERT(Context != NULL);

	if (SDL_GL_GetCurrentContext() == Context)
		SDL_GL_MakeCurrent(Window, NULL);

	SGL_LOGD("Destroy OpenGL context...\n");
	SDL_GL_DeleteContext(Context);

	SGL_LOGD("Destroy OpenGL window...\n");
	SDL_DestroyWindow(Window);

	SGL_LOGI("OpenGL shutdown.\n");
}

void DrawBegin(void)
{
	SGL_ASSERT(Window != NULL);
	SGL_ASSERT(Context != NULL);

	SDL_GL_MakeCurrent(Window, Context);
}

void DrawEnd(void)
{
	SGL_ASSERT(Window != NULL);

	SDL_GL_SwapWindow(Window);
}

SDL_Window *GetWindow(void)
{
	SGL_ASSERT(Window != NULL);

	return Window;
}

Uint32 GetWindowID(void)
{
	SGL_ASSERT(Window != NULL);

	return WindowID;
}

SDL_GLContext GetContext(void)
{
	SGL_ASSERT(Context != NULL);

	return Context;
}

SGL_GL_END
