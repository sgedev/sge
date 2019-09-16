//
//
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <GLXW/glxw.h>

#include <sge/player.hpp>

SGE_BEGIN

player::player(uv_loop_t *lp)
	: subsystem(lp)
	, m_window(NULL)
	, m_gl(NULL)
	, m_imgui(NULL)
	, m_glex(NULL)
{
	uv_timer_init(loop(), &m_frame_timer);
	uv_timer_init(loop(), &m_state_timer);

	m_frame_timer.data = this;
	m_state_timer.data = this;
}

player::~player(void)
{
}

bool player::init(void)
{
	SGE_ASSERT(m_window == NULL);
	SGE_ASSERT(m_gl == NULL);

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

	m_flags = 0;
	m_title = "SGE";

	m_window = SDL_CreateWindow(m_title.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	if (m_window == NULL)
		return false;

	m_id = SDL_GetWindowID(m_window);
	SDL_GetWindowPosition(m_window, &m_rect[0], &m_rect[1]);
	SDL_GetWindowSize(m_window, &m_rect[2], &m_rect[3]);

	SGE_LOGD("initializing opengl...\n");

	m_gl = SDL_GL_CreateContext(m_window);
	if (m_gl == NULL) {
		SGE_LOGE("failed to create opengl context.\n");
		SDL_DestroyWindow(m_window);
		m_window = NULL;
		return false;
	}

	SDL_GL_MakeCurrent(m_window, m_gl);

    IMGUI_CHECKVERSION();
    m_imgui = ImGui::CreateContext();
	ImGui::SetCurrentContext(m_imgui);

    ImGuiIO &io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(m_window, m_gl);
    ImGui_ImplOpenGL3_Init("#version 130");

	m_glex = glexCreateContext(NULL);
	if (m_glex == NULL) {
		SGE_LOGE("failed to initialize glex.\n");
		SDL_GL_DeleteContext(m_gl);
		m_gl = NULL;
		SDL_DestroyWindow(m_window);
		m_window = NULL;
		return false;
	}

	glexMakeCurrent(m_glex);

	if (!m_game.init()) {
		SGE_LOGE("failed to initialize glex.\n");
		glexDeleteContext(m_glex);
		m_glex = NULL;
		SDL_GL_DeleteContext(m_gl);
		m_gl = NULL;
		SDL_DestroyWindow(m_window);
		m_window = NULL;
		return false;
	}

	m_game.trap_fps = [this](void) { return this->m_fps; };
	m_game.trap_editor_enabled = [](void) { return false; };

	uv_timer_start(&m_frame_timer, &player::frame_cb, 0, 10);
	uv_timer_start(&m_state_timer, &player::state_cb, 1000, 1000);

	uv_update_time(loop());
	m_last = uv_now(loop());

	return true;
}

void player::shutdown(void)
{
	uv_timer_stop(&m_state_timer);
	uv_timer_stop(&m_frame_timer);

	m_game.shutdown();

	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_gl != NULL);

	glexDeleteContext(m_glex);
	m_glex = NULL;

    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext(m_imgui);
	m_imgui = NULL;

	SDL_GL_MakeCurrent(m_window, m_gl);
    ImGui_ImplOpenGL3_Shutdown();
	SDL_GL_MakeCurrent(m_window, NULL);

	SDL_GL_DeleteContext(m_gl);
	m_gl = NULL;

	SDL_DestroyWindow(m_window);
	m_window = NULL;
}

bool player::handle_event(const SDL_Event *event)
{
	SGE_ASSERT(event != NULL);

	if (event->window.windowID != m_id)
		return false;

	switch (event->type) {
	case SDL_WINDOWEVENT:
		switch (event->window.event) {
		case SDL_WINDOWEVENT_MOVED:
			m_rect.x = event->window.data1;
			m_rect.y = event->window.data2;
			return true;
		case SDL_WINDOWEVENT_RESIZED:
			m_rect.z = event->window.data1;
			m_rect.w = event->window.data2;
			return true;
		case SDL_WINDOWEVENT_EXPOSED:
		case SDL_WINDOWEVENT_SHOWN:
			m_flags |= FLAG_VISIBLED;
			return true;
		case SDL_WINDOWEVENT_HIDDEN:
		case SDL_WINDOWEVENT_MINIMIZED:
			m_flags &= ~FLAG_VISIBLED;
			return true;
		}
		break;
	case SDL_KEYDOWN:
		if (event->key.keysym.sym == SDLK_RETURN && event->key.keysym.mod & KMOD_ALT) {
			if (m_flags & FLAG_FULLSCREEN) {
				m_flags &= ~FLAG_FULLSCREEN;
				SDL_SetWindowFullscreen(m_window, 0);
			} else {
				m_flags |= FLAG_FULLSCREEN;
				SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
			}
			return true;
		}
		break;
	}

	ImGui::SetCurrentContext(m_imgui);
	ImGui_ImplSDL2_ProcessEvent(event);

	return m_game.handle_event(event);
}

void player::update(float elapsed)
{
	m_game.update(elapsed);
	m_game.draw();
}

void player::render(void)
{
	if (!(m_flags & FLAG_VISIBLED))
		return;

	if (m_rect[2] < 1 || m_rect[3] < 1)
		return;

	if (SDL_GL_MakeCurrent(m_window, m_gl))
		return;

	glViewport(0, 0, m_rect[2], m_rect[3]);
	glClear(GL_COLOR_BUFFER_BIT);

	glexRender();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(m_window);
}

void player::frame_cb(uv_timer_t *p)
{
	SGE_ASSERT(p != NULL);
	SGE_ASSERT(p->data != NULL);

	((player *)(p->data))->frame();
}

void player::frame(void)
{
	glexMakeCurrent(m_glex);
	glexClear();

	ImGui::SetCurrentContext(m_imgui);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_window);
    ImGui::NewFrame();

	uint64_t elapsed = uv_now(loop()) - m_last;
	update(float(elapsed) / 1000.0f);

	ImGui::Render();

	render();

	m_fps_count += 1;
	uv_update_time(loop());
	m_last = uv_now(loop());
}

void player::state_cb(uv_timer_t *p)
{
	SGE_ASSERT(p != NULL);
	SGE_ASSERT(p->data != NULL);

	((player *)(p->data))->state();
}

void player::state(void)
{
	m_fps = m_fps_count;
	m_fps_count = 0;
}

SGE_END

