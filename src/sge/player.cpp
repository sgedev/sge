//
//
#include <sge/player.hpp>

SGE_BEGIN

player::player(uv_loop_t *loop)
	: m_loop(loop ? loop : uv_default_loop())
	, m_imgui(NULL)
	, m_flags(0)
	, m_draw_fps(true)
	, m_draw_hud(true)
{
	uv_timer_init(m_loop, &m_frame_timer);
	uv_timer_init(m_loop, &m_state_timer);

	m_frame_timer.data = this;
	m_state_timer.data = this;
}

player::~player(void)
{
	if (started())
		stop();
}

bool player::start(void)
{
	SGE_ASSERT(m_loop != NULL);
	SGE_ASSERT(!started());

	if (!init())
		return false;

	m_flags |= FLAG_STARTED;

	return true;
}

void player::stop(void)
{
	SGE_ASSERT(m_loop != NULL);
	SGE_ASSERT(started());

	shutdown();

	m_flags &= ~FLAG_STARTED;
}

void player::feed_event(const SDL_Event &event)
{
	SGE_ASSERT(started());

	handle_event(event);
}

bool player::init(void)
{
	SGE_ASSERT(m_loop != NULL);
	SGE_ASSERT(m_imgui == NULL);

	if (!m_game.init(this))
		return false;

	if (!m_main_window.init()) {
		m_game.shutdown();
		return false;
	}

	if (!m_renderer.init(&m_main_window)) {
		m_main_window.shutdown();
		m_game.shutdown();
		return false;
	}

	if (!init_imgui()) {
		m_renderer.shutdown();
		m_main_window.shutdown();
		m_game.shutdown();
		return false;
	}

	m_view.perspective(90, 4.0f/3.0f, 0.01f, 100.0f);

	uv_timer_start(&m_frame_timer, &player::frame_cb, 10, 10);
	uv_timer_start(&m_state_timer, &player::state_cb, 0, 1000);

	m_fps = 0;
	m_fps_count = 0;
	m_last = uv_now(m_loop);
	m_flags = 0;
	m_draw_fps = true;
	m_draw_hud = true;

	return true;
}

void player::shutdown(void)
{
	uv_timer_stop(&m_frame_timer);
	uv_timer_stop(&m_state_timer);

	m_game.shutdown();

	shutdown_imgui();

	m_renderer.shutdown();
	m_main_window.shutdown();
}

void player::handle_event(const SDL_Event &event)
{
	SGE_ASSERT(started());
	SGE_ASSERT(m_imgui != NULL);

	m_main_window.handle_event(event);

	ImGui::SetCurrentContext(m_imgui);
	ImGui_ImplSDL2_ProcessEvent(&event);

	m_game.handle_event(event);
}

void player::update(float elapsed)
{
	m_game.update(elapsed);
}

unsigned int player::trap_fps(void)
{
	return fps();
}

bool player::init_imgui(void)
{
	IMGUI_CHECKVERSION();

	m_imgui = ImGui::CreateContext();
	if (m_imgui == NULL) {
		SGE_LOGE("failed to create dear imgui context.\n");
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

	ImGui_ImplSDL2_InitForOpenGL(m_main_window.to_sdl_window(), m_main_window.sdl_gl_context());
	ImGui_ImplOpenGL3_Init("#version 130");

	return true;
}

void player::shutdown_imgui(void)
{
	SGE_ASSERT(m_imgui != NULL);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();

	ImGui::SetCurrentContext(NULL);
	ImGui::DestroyContext(m_imgui);
	m_imgui = NULL;
}

void player::draw_loading(void)
{
	if (!ImGui::IsPopupOpen("Loading..."))
		ImGui::OpenPopup("Loading...");

	if (!ImGui::BeginPopupModal("Loading...", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		return;

	ImGui::NewLine();
	ImGui::ProgressBar(0.3f, ImVec2(400, 20));
	ImGui::NewLine();

	//ImGui::CloseCurrentPopup();
	ImGui::EndPopup();
}

void player::draw_ready(void)
{
}

void player::draw_fps(void)
{
	if (!m_draw_fps)
		return;

	ImGui::Begin("fps", &m_draw_fps, ImGuiWindowFlags_AlwaysAutoResize);

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

void player::draw_hud(void)
{
	if (!m_draw_hud)
		return;
}

void player::frame(void)
{
	SGE_ASSERT(m_loop != NULL);
	SGE_ASSERT(m_imgui != NULL);

	ImGui::SetCurrentContext(m_imgui);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_main_window.to_sdl_window());
	ImGui::NewFrame();

	uint64_t pass = uv_now(m_loop) - m_last;
	float elapsed = float(pass) / 1000.0f;

	update(elapsed);

	m_view.reset();

	switch (m_game.current_state()) {
	case game::STATE_IDLE:
		break;
	case game::STATE_LOADING:
		draw_loading();
		break;
	case game::STATE_READY:
		draw_ready();
		break;
	case game::STATE_PLAYING:
		m_game.draw(m_view);
		break;
	}

	draw_hud();
	draw_fps();

	ImGui::Render();

	if (m_main_window.draw_begin()) {
		m_renderer.render(m_view);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		m_main_window.draw_end();
	}

	m_fps_count += 1;
	uv_update_time(m_loop);
	m_last = uv_now(m_loop);
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

SGE_END

