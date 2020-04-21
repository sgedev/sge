/*
 *
 */
#include <sge/window.h>

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

#define SGE_WINDOW_VISIBLE 0x1
#define SGE_WINDOW_FOCUS 0x2

static SDL_Window *SGE_Window;
static Uint32 SGE_WindowID;
static int SGE_WindowWidth;
static int SGE_WindowHeight;
static int SGE_WindowFlags;
static SDL_GLContext SGE_GLContext;
static GLEXContext *SGE_GLEXContext;
static NVGcontext *SGE_NVGContext;

int SGE_InitWindow(void)
{
	int ret;
	int flags;

	CX_ASSERT(SGE_Window == NULL);
	CX_ASSERT(SGE_GLContext == NULL);
	CX_ASSERT(SGE_GLEXContext == NULL);
	CX_ASSERT(SGE_NVGContext == NULL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &flags);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, flags | SDL_GL_CONTEXT_DEBUG_FLAG);

	SGE_Window = SDL_CreateWindow("SGE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	if (SGE_Window == NULL)
		goto bad0;
	
	SGE_WindowID = SDL_GetWindowID(SGE_Window);
	SGE_WindowWidth = 800;
	SGE_WindowHeight = 600;
	SGE_WindowFlags = SGE_WINDOW_VISIBLE | SGE_WINDOW_FOCUS;

	SGE_GLContext = SDL_GL_CreateContext(SGE_Window);
	if (SGE_GLContext == NULL)
		goto bad1;

	SDL_GL_MakeCurrent(SGE_Window, SGE_GLContext);

	if (glewInit() != GLEW_OK)
		goto bad2;
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	SGE_GLEXContext = glexCreateContext();
	if (SGE_GLEXContext == NULL)
		goto bad2;
	
	glexMakeCurrent(SGE_GLContext);
	glexRenderMode(GLEX_RENDER_MODE_FORWARD);

	SGE_NVGContext = nvgCreateGL3(0);
	if (SGE_NVGContext == NULL)
		goto bad3;

	return 0;

bad3:
	glexDeleteContext(SGE_GLEXContext);
	SGE_GLEXContext = NULL;

bad2:
	SDL_GL_DeleteContext(SGE_GLContext);
	SGE_GLContext = NULL;

bad1:
	SDL_DestroyWindow(SGE_Window);
	SGE_Window = NULL;

bad0:
	return -1;
}

void SGE_ShutdownWindow(void)
{
	CX_ASSERT(SGE_Window != NULL);
	CX_ASSERT(SGE_GLContext != NULL);
	CX_ASSERT(SGE_GLEXContext != NULL);
	CX_ASSERT(SGE_NVGContext != NULL);

	nvgDeleteGL3(SGE_NVGContext);
	SGE_NVGContext = NULL;

	glexDeleteContext(SGE_GLEXContext);
	SGE_GLEXContext = NULL;

	SDL_GL_DeleteContext(SGE_GLContext);
	SGE_GLContext = NULL;

	SDL_DestroyWindow(SGE_Window);
	SGE_Window = NULL;
}

int SGE_HandleWindowEvent(const SDL_WindowEvent *event)
{
	switch (event->event) {
	case SDL_WINDOWEVENT_SHOWN:
		SGE_WindowFlags |= SGE_WINDOW_VISIBLE;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		SGE_WindowFlags &= ~SGE_WINDOW_VISIBLE;
		break;
	case SDL_WINDOWEVENT_RESIZED:
		SGE_WindowWidth = event->data1;
		SGE_WindowHeight = event->data2;
		break;
	case SDL_WINDOWEVENT_FOCUS_GAINED:
		SGE_WindowFlags |= SGE_WINDOW_FOCUS;
		break;
	case SDL_WINDOWEVENT_FOCUS_LOST:
		SGE_WindowFlags &= ~SGE_WINDOW_FOCUS;
		break;
	}
}

void SGE_RenderWindow(const SGE_WindowDrawer *drawer)
{
	CX_ASSERT(drawer != NULL);
	CX_ASSERT(SGE_Window != NULL);
	CX_ASSERT(SGE_GLContext != NULL);
	CX_ASSERT(SGE_GLEXContext != NULL);
	CX_ASSERT(SGE_NVGContext != NULL);

	if (!(SGE_WindowFlags & SGE_WINDOW_VISIBLE))
		return;

	if (SGE_WindowWidth < 1 || SGE_WindowHeight < 1)
		return;

	SDL_GL_MakeCurrent(SGE_Window, SGE_GLContext);

	if (drawer->Draw3D != NULL) {
		glexBeginFrame();
		drawer->Draw3D();
		glexEndFrame();
	}

	if (drawer->Draw2D != NULL) {
		glexBeginFrame();
		drawer->Draw2D(SGE_NVGContext);
		glexEndFrame();
	}

	SDL_GL_SwapWindow(SGE_Window);
}

