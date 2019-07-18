//
//
#include <sge/player.hpp>

SGE_BEGIN

player::player(uv_loop_t *lp)
	: subsystem(lp)
	, m_imgui(NULL)
	, m_flags(0)
	, m_game(lp)
	, m_show_fps(true)
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
	SGE_ASSERT(!started());
	SGE_ASSERT(m_imgui == NULL);

	if (!m_window.init())
		return false;

	IMGUI_CHECKVERSION();

    m_imgui = ImGui::CreateContext();
	if (m_imgui == NULL) {
		SGE_LOGE("failed to create imgui context.\n");
		m_window.shutdown();
		return false;
	}

	ImGui::SetCurrentContext(m_imgui);

	SGE_LOGI("dear imgui: %s\n", ImGui::GetVersion());

    ImGuiIO &io = ImGui::GetIO();
	io.IniFilename = NULL;

    ImGui::StyleColorsDark();

	ImGuiStyle &style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 0.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 0.0f;

    ImGui_ImplSDL2_InitForOpenGL(m_window.to_sdl_window(), m_window.sdl_gl_context());
    ImGui_ImplOpenGL3_Init("#version 130");

	if (!m_game.init(&m_window)) {
		SGE_LOGE("failed to initialize game.\n");
		ImGui::DestroyContext(m_imgui);
		m_imgui = NULL;
		m_window.shutdown();
		return false;
	}

	uv_timer_start(&m_frame_timer, &player::frame_cb, 10, 10);
	uv_timer_start(&m_state_timer, &player::state_cb, 0, 1000);

	m_fps = 0;
	m_fps_count = 0;

	m_last = uv_now(loop());

	return true;
}

void player::shutdown(void)
{
	uv_timer_stop(&m_frame_timer);
	uv_timer_stop(&m_state_timer);

	m_game.shutdown();

	SGE_ASSERT(m_imgui != NULL);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();

	ImGui::SetCurrentContext(NULL);
	ImGui::DestroyContext(m_imgui);
	m_imgui = NULL;

	m_window.shutdown();
}

void player::handle_event(const SDL_Event &event)
{
	SGE_ASSERT(started());

	m_window.handle_event(event);
	m_game.handle_event(event);

	ImGui::SetCurrentContext(m_imgui);
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void player::update(float elapsed)
{
}

void player::draw(void)
{
}

void player::frame(void)
{
	SGE_ASSERT(m_imgui != NULL);

	uint64_t pass = uv_now(loop()) - m_last;
	float elapsed = float(pass) / 1000.0f;

	ImGui::SetCurrentContext(m_imgui);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window.to_sdl_window());
	ImGui::NewFrame();

	m_game.update(elapsed);
	update(elapsed);

	show_fps();

	ImGui::Render();

	if (m_window.draw_begin()) {
		m_game.draw();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		draw();
		m_window.draw_end();
	}

	m_fps_count += 1;
	uv_update_time(loop());
	m_last = uv_now(loop());
}

void player::frame_cb(uv_timer_t *timer)
{
	((player *)(timer->data))->frame();
}

void player::state(void)
{
	m_fps = m_fps_count;
	m_fps_count = 0;
}

void player::state_cb(uv_timer_t *timer)
{
	((player *)(timer->data))->state();
}

void player::show_fps(void)
{
	if (!m_show_fps)
		return;

	ImGui::Begin("fps", &m_show_fps, ImGuiWindowFlags_AlwaysAutoResize);

	ImVec4 fps_color;

	if (m_fps > 59)
		fps_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	else if (m_fps < 30)
		fps_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	else
		fps_color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);

	ImGui::TextColored(fps_color, "%10d", m_fps);
	ImGui::End();
}

SGE_END

