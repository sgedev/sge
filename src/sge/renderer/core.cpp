//
//
#include <sge/renderer.hpp>

SGE_RENDERER_BEGIN

static SDL_Window *glWindow;
static SDL_GLContext glContext;
static int glWindowSize[2];
static Uint32 glWindowID;
static bool glWindowVisible;

bool init(void)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	logInfo("Creating main window...");

	SGE_ASSERT(glWindow == NULL);
	glWindow = SDL_CreateWindow("SGE",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN);
	if (glWindow == NULL) {
		logError("Failed to create main window.\n");
		goto bad0;
	}

	glWindowVisible = true;
	glWindowID = SDL_GetWindowID(glWindow);
	SDL_GetWindowSize(glWindow, &glWindowSize[0], &glWindowSize[1]);

	logInfo("Create OpenGL context...\n");

	SGE_ASSERT(glContext == NULL);
	glContext = SDL_GL_CreateContext(glWindow);
	if (glContext == NULL) {
		logError("Failed to create OpenGL context.\n");
		goto bad1;
	}

	SDL_GL_MakeCurrent(glWindow, glContext);

	int ret = glewInit();
	if (ret != GLEW_OK) {
		logError("Failed to initialize GLEW.\n");
		goto bad2;
	}

	logInfo("GLEW version: %s\n", glewGetString(GLEW_VERSION));
	logInfo("OpenGL version: %s\n", glGetString(GL_VERSION));

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	return true;

bad2:
	SDL_GL_DeleteContext(glContext);
	glContext = NULL;

bad1:
	SDL_DestroyWindow(glWindow);
	glWindow = NULL;

bad0:
	return false;
}

void shutdown(void)
{
	SGE_ASSERT(glContext != NULL);
	SDL_GL_DeleteContext(glContext);
	glContext = NULL;

	SGE_ASSERT(glWindow != NULL);
	SDL_DestroyWindow(glWindow);
	glWindow = NULL;
}

bool handleEvent(const SDL_Event &event)
{
	SGE_ASSERT(glWindow != NULL);

	if (event.type != SDL_WINDOWEVENT)
		return false;

	if (event.window.windowID != glWindowID)
		return false;

	switch (event.window.type) {
	case SDL_WINDOWEVENT_SHOWN:
		glWindowVisible = true;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		glWindowVisible = false;
		break;
	case SDL_WINDOWEVENT_RESIZED:
		glWindowSize[0] = event.window.data1;
		glWindowSize[1] = event.window.data2;
		break;
	default:
		return false;
	}

	return true;
}

bool beginFrame(void)
{
	SGE_ASSERT(glWindow != NULL);

	if (!glWindowVisible || glWindowSize[0] < 1 || glWindowSize[1] < 1)
		return false;

	if (SDL_GL_MakeCurrent(glWindow, glContext) < 0)
		return false;

	glViewport(0, 0, glWindowSize[0], glWindowSize[1]);
	glClear(GL_COLOR_BUFFER_BIT);

	return true;
}

void endFrame(void)
{
	SGE_ASSERT(glWindow != NULL);
	SGE_ASSERT(SDL_GL_GetCurrentWindow() == glWindow);
	SGE_ASSERT(SDL_GL_GetCurrentContext() == glContext);

	SDL_GL_SwapWindow(glWindow);
}

SGE_RENDERER_END
