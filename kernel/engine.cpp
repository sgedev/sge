//
//
#include <sge/engine.hpp>

SGE_BEGIN

engine::engine(uv_loop_t *loop)
	: m_loop(loop != NULL ? loop : uv_default_loop())
	, m_time_base(0)
	, m_time_scale(1.0f)
	, m_fps_count(0)
	, m_fps(0)
	, m_started(false)
{
	SGE_ASSERT(m_loop != NULL);

	uv_timer_init(m_loop, &m_frame_timer);
	uv_timer_init(m_loop, &m_state_timer);

	m_frame_timer.data = this;
	m_state_timer.data = this;
}

engine::~engine(void)
{
	if (m_started)
		stop();
}

bool engine::start(void)
{
	SGE_ASSERT(m_loop != NULL);
	SGE_ASSERT(!m_started);

	if (!init())
		return false;

	uv_timer_start(&m_frame_timer, &engine::frame_cb, 0, 16);
	uv_timer_start(&m_state_timer, &engine::state_cb, 1000, 1000);

	m_time_base = uv_now(m_loop);
	m_time_scale = 1.0f;
	m_fps_count = 0;
	m_fps = 0;
	m_started = true;

	return true;
}

void engine::stop(void)
{
	SGE_ASSERT(m_started);

	uv_timer_stop(&m_frame_timer);
	uv_timer_stop(&m_state_timer);

	shutdown();

	m_started = false;
}

void engine::handle_event(const SDL_Event &event)
{
	switch (event.type) {
	case SDL_WINDOWEVENT:
		handle_window_event(event.window);
		break;
	default:
		break;
	}
}

bool engine::init(void)
{
	SGE_LOGD("Creating OpenGL window...\n");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

#ifdef SGE_DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	m_window = SDL_CreateWindow(SGE_GAME_NAME,
		SDL_WINDOWPOS_CENTERED,	SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (m_window == NULL)
		return false;

	SGE_LOGD("Initializing OpenGL...\n");

	m_gl_context = SDL_GL_CreateContext(m_window);
	if (m_gl_context == NULL) {
		SDL_DestroyWindow(m_window);
		SGE_LOGE("Failed to create OpenGL context.\n");
		return false;
	}

	SDL_GL_MakeCurrent(m_window, m_gl_context);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SDL_GL_DeleteContext(m_gl_context);
		SDL_DestroyWindow(m_window);
		SGE_LOGE("Failed to initialize GLEW.\n");
		return false;
	}

#ifdef SGE_DEBUG
	//if (GL_KHR_debug) {
	//	SGE_LOGD("Enable OpenGL debug output.\n");
	//	glDebugMessageCallback(debug_output, NULL);
	//}
#endif

	SGE_LOGI("OpenGL: %s\n", glGetString(GL_VERSION));

	m_window_id = SDL_GetWindowID(m_window);

	m_gui.init();
	m_scene.init();

	return true;
}

void engine::shutdown(void)
{
	m_gui.shutdown();
	m_scene.shutdown();

	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_gl_context != NULL);

	if (m_gl_context == SDL_GL_GetCurrentContext())
		SDL_GL_MakeCurrent(m_window, NULL);

	SDL_GL_DeleteContext(m_gl_context);
	m_gl_context = NULL;

	SDL_DestroyWindow(m_window);
	m_window = NULL;
}

void engine::update(float elapsed)
{
	m_scene.update(elapsed);
	m_gui.update(elapsed);
}

void engine::draw(void)
{
	m_scene.draw();
	m_gui.draw();
}

void engine::frame(void)
{
	SGE_ASSERT(m_loop != NULL);

	float elapsed = float(uv_now(m_loop) - m_time_base);

	elapsed *= m_time_scale;
	elapsed /= 1000.0f;

	update(elapsed);
	
	if (m_visibled && SDL_GL_MakeCurrent(m_window, m_gl_context) == 0) {
		draw();
		SDL_GL_SwapWindow(m_window);
	}

	uv_update_time(m_loop);

	m_time_base = uv_now(m_loop);
	m_fps_count++;
}

void engine::frame_cb(uv_timer_t *p)
{
	((engine *)(p->data))->frame();
}

void engine::state(void)
{
	m_fps = m_fps_count;
	m_fps_count = 0;

	// printf("fps %d\n", m_fps);
}

void engine::state_cb(uv_timer_t *p)
{
	((engine *)(p->data))->state();
}

void engine::handle_window_event(const SDL_WindowEvent &event)
{
	if (event.windowID != m_window_id)
		return;

	switch (event.event) {
	case SDL_WINDOWEVENT_SHOWN:
		m_visibled = true;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		m_visibled = false;
		break;
	}
}

SGE_END

