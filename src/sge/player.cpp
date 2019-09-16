//
//
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <sge/player.hpp>

SGE_BEGIN

player::player(uv_loop_t *lp)
	: subsystem(lp)
	, m_imgui(NULL)
	, m_glex(NULL)
	, m_last(0)
	, m_fps(0)
	, m_fps_count(0)
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
	if (!m_window.init())
		return false;

	if (!m_window.make_current()) {
		m_window.shutdown();
		return false;
	}

	m_glex = glexCreateContext(NULL);
	if (m_glex == NULL) {
		SGE_LOGE("failed to initialize glex.\n");
		m_window.shutdown();
		return false;
	}

    IMGUI_CHECKVERSION();
    m_imgui = ImGui::CreateContext();
	ImGui::SetCurrentContext(m_imgui);

    ImGuiIO &io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(m_window.sdl_window(), m_window.sdl_gl_context());
    ImGui_ImplOpenGL3_Init("#version 330");

	if (!m_game.init()) {
		SGE_LOGE("failed to initialize glex.\n");
		glexDeleteContext(m_glex);
		m_glex = NULL;
		m_window.shutdown();
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

	m_window.make_current();

	glexDeleteContext(m_glex);
	m_glex = NULL;

    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext(m_imgui);
	m_imgui = NULL;

	m_window.done_current();
	m_window.shutdown();
}

bool player::handle_event(const SDL_Event *event)
{
	SGE_ASSERT(event != NULL);

	m_window.handle_event(event);

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
	if (!m_window.visibled() || !m_window.make_current())
		return;

	glClear(GL_COLOR_BUFFER_BIT);

	glexRender();

	glViewport(0, 0, m_window.rect()[2], m_window.rect()[3]);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	m_window.swap_buffers();
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
	glexBeginFrame();
	glexViewport(0, 0, m_window.rect()[2], m_window.rect()[3]);

	ImGui::SetCurrentContext(m_imgui);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(m_window.sdl_window());
    ImGui::NewFrame();

	uint64_t elapsed = uv_now(loop()) - m_last;
	update(float(elapsed) / 1000.0f);

	ImGui::Render();
	glexEndFrame();

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

