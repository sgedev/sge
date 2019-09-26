//
//
#include "game.hpp"

Game::Game(void)
{
}

Game::~Game(void)
{
}

bool Game::Init(void)
{
	SGE_ASSERT(m_window == NULL);

	if (!m_game.init())
		goto bad0;

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

	m_window = SDL_CreateWindow("SGE",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

	if (m_window == NULL)
		goto bad1;

	m_window_id = SDL_GetWindowID(m_window);
	SDL_GetWindowPosition(m_window, &m_window_rect[0], &m_window_rect[1]);
	SDL_GetWindowSize(m_window, &m_window_rect[2], &m_window_rect[3]);

	m_gl = SDL_GL_CreateContext(m_window);
	if (m_gl == NULL)
		goto bad2;

	SDL_GL_MakeCurrent(m_window, m_gl);

	if (gl3wInit(&m_gl3w, (GL3WGetProcAddressProc)SDL_GL_GetProcAddress) != GL3W_OK)
		goto bad3;

	gl3wProcs = &m_gl3w;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	if (!m_glex.Init())
		goto bad3;

	if (!m_view.init(&m_glex))
		goto bad4;

	return true;

bad4:
	m_glex.Shutdown();

bad3:
	SDL_GL_DeleteContext(m_gl);
	m_gl = NULL;

bad2:
	SDL_DestroyWindow(m_window);
	m_window = NULL;

bad1:
	m_game.shutdown();

bad0:
	return false;
}

void Game::Shutdown(void)
{
	SGE_ASSERT(m_window != NULL);

	m_glex.Shutdown();

	SDL_GL_DeleteContext(m_gl);
	m_gl = NULL;

	SDL_DestroyWindow(m_window);
	m_window = NULL;

	m_game.shutdown();
}

void Game::Frame(float elapsed)
{
	m_game.update(elapsed);

	m_glex.BeginFrame();

	glClear(GL_COLOR_BUFFER_BIT);

	m_game.draw(&m_view);

	m_glex.EndFrame();

	SDL_GL_SwapWindow(m_window);
}

