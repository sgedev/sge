//
//
#include <SGE/Player.hpp>

SGE_BEGIN

Player::Player(uv_loop_t *loop):
	Handle(loop),
	m_window(NULL),
	m_context(NULL),
	m_imgui_sdl2(NULL),
	m_imgui_opengl3(NULL),
	m_imgui(NULL)
{
	SGE_ASSERT(loop != NULL);

	uv_timer_init(loop, &m_frameTimer);
	m_frameTimer.data = this;

	uv_timer_init(loop, &m_stateTimer);
	m_stateTimer.data = this;
}

Player::~Player(void)
{
}

bool Player::start(const std::string &path)
{
	SGE_ASSERT(m_window == NULL);
	SGE_ASSERT(m_context == NULL);
	SGE_ASSERT(m_imgui == NULL);
	SGE_ASSERT(m_imgui_sdl2 == NULL);
	SGE_ASSERT(m_imgui_opengl3 == NULL);

	if (!Handle::start(path))
		return false;

	if (!m_scene.init()) {
		Handle::stop();
		return false;
	}

	if (!initWindow()) {
		Handle::stop();
		return false;
	}

	SDL_GL_MakeCurrent(m_window, m_context);

	if (gl3wInit(&m_gl3w, (GL3WGetProcAddressProc)SDL_GL_GetProcAddress) < 0) {
		SGE_LOGE("Failed to init OpenGL procs.");
		releaseWindow();
		Handle::stop();
		return false;
	}

	gl3wProcs = &m_gl3w;

	SGE_LOGI("OpenGL: %s", glGetString(GL_VERSION));
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

#if 0 // defined SGE_DEBUG && defined SGE_LOG
	if (GL_KHR_debug)
		glDebugMessageCallback(sge_gl_debug_output, NULL);
#endif

	m_imgui = ImGui::CreateContext();

	ImGui::SetCurrentContext(m_imgui);
	m_imgui_sdl2 = ImGui_ImplSDL2_NewForOpenGL(m_window, m_context);
	m_imgui_opengl3 = ImGui_ImplOpenGL3_New("#version 130");

	m_renderer.init();
	m_scene.addListener(&m_renderer);

	m_physicsWorld.init();
	m_scene.addListener(&m_physicsWorld);

	uv_timer_start(&m_frameTimer, &Player::frameCallback, 16, 16);
	uv_timer_start(&m_stateTimer, &Player::stateCallback, 1000, 1000);

	m_fps = 0;
	m_fpsCount = 0;
	m_last = SDL_GetTicks();

	return true;
}

void Player::stop(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_context != NULL);
	SGE_ASSERT(m_imgui != NULL);
	SGE_ASSERT(m_imgui_sdl2 != NULL);
	SGE_ASSERT(m_imgui_opengl3 != NULL);

	Handle::stop();

	uv_timer_stop(&m_stateTimer);
	uv_timer_stop(&m_frameTimer);

	SDL_GL_MakeCurrent(m_window, m_context);

	ImGui_ImplOpenGL3_Destroy(m_imgui_opengl3);
	ImGui_ImplSDL2_Destroy(m_imgui_sdl2);
	ImGui::DestroyContext(m_imgui);

	releaseWindow();
}

bool Player::handleEvent(const SDL_Event &event)
{
	if (event.type == SDL_WINDOWEVENT)
		return handleWindowEvent(event.window);

	ImGui::SetCurrentContext(m_imgui);
	ImGui_ImplSDL2_MakeCurrent(m_imgui_sdl2);

	if (ImGui_ImplSDL2_ProcessEvent(&event))
		return true;

	return false;
}

bool Player::initWindow(void)
{
	SGE_ASSERT(m_window == NULL);
	SGE_ASSERT(m_context == NULL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#ifdef SGE_DEBUG
	int glFlags;
	SGE_LOGI("OpenGL debug enabled.");
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &glFlags);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, glFlags | SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	SGE_LOGD("Creating main window...");

	m_window = SDL_CreateWindow("SGE",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	if (m_window == NULL) {
		SGE_LOGE("Failed to create main window.");
		return false;
	}

	m_visibled = true;
	m_id = SDL_GetWindowID(m_window);
	SDL_GetWindowPosition(m_window, &m_rect[0], &m_rect[1]);
	SDL_GetWindowSize(m_window, &m_rect[2], &m_rect[3]);

	SGE_LOGD("Create OpenGL context...");
	m_context = SDL_GL_CreateContext(m_window);
	if (m_context == NULL) {
		SDL_DestroyWindow(m_window);
		m_window = NULL;
		SGE_LOGE("Failed to create OpenGL context.");
		return false;
	}

	return true;
}

void Player::releaseWindow(void)
{
	SGE_ASSERT(m_context != NULL);
	SDL_GL_DeleteContext(m_context);
	m_context = NULL;

	SGE_ASSERT(m_window != NULL);
	SDL_DestroyWindow(m_window);
	m_window = NULL;
}

bool Player::handleWindowEvent(const SDL_WindowEvent &event)
{
	if (event.windowID != m_id)
		return false;

	switch (event.event) {
	case SDL_WINDOWEVENT_SHOWN:
		m_visibled = true;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		m_visibled = false;
		break;
	case SDL_WINDOWEVENT_MOVED:
		m_rect[0] = event.data1;
		m_rect[1] = event.data2;
		break;
	case SDL_WINDOWEVENT_RESIZED:
		m_rect[2] = event.data1;
		m_rect[3] = event.data2;
		break;
	case SDL_WINDOWEVENT_FOCUS_GAINED:
		break;
	case SDL_WINDOWEVENT_FOCUS_LOST:
		break;
	default:
		return false;
	}

	return true;
}

void Player::updateWindow(void)
{
	if (!m_visibled)
		return;

	if (m_rect[2] < 1 || m_rect[3] < 1)
		return;

	if (SDL_GL_MakeCurrent(m_window, m_context) < 0)
		return;

	ImGui::SetCurrentContext(m_imgui);

	ImGui_ImplSDL2_MakeCurrent(m_imgui_sdl2);
	ImGui_ImplOpenGL3_MakeCurrent(m_imgui_opengl3);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();
	updateGui();
	ImGui::Render();

	glClear(GL_COLOR_BUFFER_BIT);
	m_renderer.draw();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(m_window);
}

void Player::updateGui(void)
{
}

void Player::frame(void)
{
	Uint32 curr = SDL_GetTicks() - m_last;
	float elapsed = float(curr) / 1000.0f;

	m_scene.update(elapsed);
	m_physicsWorld.update(elapsed);
	m_renderer.update(elapsed);

	updateWindow();

	uv_update_time(loop());
}

void Player::frameCallback(uv_timer_t *p)
{
	((Player *)(p->data))->frame();
}

void Player::state(void)
{
	m_fps = m_fpsCount;
	m_fpsCount = 0;
}

void Player::stateCallback(uv_timer_t *p)
{
	((Player *)(p->data))->state();
}

SGE_END
