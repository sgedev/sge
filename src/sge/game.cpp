//
//
#include <sge/game.hpp>

SGE_BEGIN

game::game(uv_loop_t *loop, const filesystem::path &path)
	: m_loop(loop ? loop : uv_default_loop())
	, m_path(path)
	, m_imgui(NULL)
	, m_flags(0)
	, m_show_fps(true)
	, m_show_hud(true)
{
	m_path = path.make_absolute();
	SGE_ASSERT(m_path.exists());

	uv_timer_init(m_loop, &m_frame_timer);
	uv_timer_init(m_loop, &m_state_timer);

	m_frame_timer.data = this;
	m_state_timer.data = this;
}

game::~game(void)
{
	if (started())
		stop();
}

bool game::start(void)
{
	SGE_ASSERT(m_loop != NULL);
	SGE_ASSERT(!started());

	if (!init())
		return false;

	m_flags |= FLAG_STARTED;

	return true;
}

void game::stop(void)
{
	SGE_ASSERT(m_loop != NULL);
	SGE_ASSERT(started());

	shutdown();

	m_flags &= ~FLAG_STARTED;
}

void game::feed_event(const SDL_Event &event)
{
	SGE_ASSERT(started());

	handle_event(event);
}

bool game::init(void)
{
	SGE_ASSERT(m_loop != NULL);
	SGE_ASSERT(m_imgui == NULL);

	if (!m_main_window.init())
		return false;

	if (!m_renderer.init(&m_main_window)) {
		m_main_window.shutdown();
		return false;
	}

	if (!init_imgui()) {
		m_renderer.shutdown();
		m_main_window.shutdown();
		return false;
	}

	m_scene.init();

	m_view.perspective(90, 4.0f/3.0f, 0.01f, 100.0f);

	m_runtime.init();

	uv_timer_start(&m_frame_timer, &game::frame_cb, 10, 10);
	uv_timer_start(&m_state_timer, &game::state_cb, 0, 1000);

	m_fps = 0;
	m_fps_count = 0;
	m_last = uv_now(m_loop);
	m_flags = 0;
	m_show_fps = true;
	m_show_hud = true;
	m_state = STATE_LOADING;

	return true;
}

void game::shutdown(void)
{
	uv_timer_stop(&m_frame_timer);
	uv_timer_stop(&m_state_timer);

	m_runtime.shutdown();
	m_scene.shutdown();

	shutdown_imgui();

	m_renderer.shutdown();
	m_main_window.shutdown();
}

void game::handle_event(const SDL_Event &event)
{
	SGE_ASSERT(started());
	SGE_ASSERT(m_imgui != NULL);

	m_main_window.handle_event(event);

	ImGui::SetCurrentContext(m_imgui);
	ImGui_ImplSDL2_ProcessEvent(&event);

	m_runtime.handle_event(event);
}

void game::update(float elapsed)
{
	SGE_ASSERT(started());
}

bool game::init_imgui(void)
{
	IMGUI_CHECKVERSION();

	m_imgui = ImGui::CreateContext();
	if (m_imgui == NULL) {
		SGE_LOGE("failed to create imgui context.\n");
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

void game::shutdown_imgui(void)
{
	SGE_ASSERT(m_imgui != NULL);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();

	ImGui::SetCurrentContext(NULL);
	ImGui::DestroyContext(m_imgui);
	m_imgui = NULL;
}

void game::show_loading(void)
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

void game::show_ready(void)
{
}

void game::show_fps(void)
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

void game::show_hud(void)
{
	if (!m_show_hud)
		return;
}

void game::frame(void)
{
	SGE_ASSERT(m_loop != NULL);
	SGE_ASSERT(m_imgui != NULL);

	ImGui::SetCurrentContext(m_imgui);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_main_window.to_sdl_window());
	ImGui::NewFrame();

	uint64_t pass = uv_now(m_loop) - m_last;
	float elapsed = float(pass) / 1000.0f;

	m_scene.update(elapsed);
	m_runtime.update(elapsed);
	update(elapsed);

	m_view.clear();

	switch (m_state) {
	case STATE_IDLE:
		break;
	case STATE_LOADING:
		show_loading();
		break;
	case STATE_READY:
		show_ready();
		break;
	case STATE_PLAYING:
		m_scene.shot(m_view);
		break;
	}

	show_hud();
	show_fps();

	ImGui::Render();

	if (m_main_window.draw_begin()) {
		m_renderer.draw(m_view);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		m_main_window.draw_end();
	}

	m_fps_count += 1;
	uv_update_time(m_loop);
	m_last = uv_now(m_loop);
}

void game::frame_cb(uv_timer_t *timer)
{
	((game *)(timer->data))->frame();
}

void game::state(void)
{
	m_fps = m_fps_count;
	m_fps_count = 0;
}

void game::state_cb(uv_timer_t *timer)
{
	((game *)(timer->data))->state();
}

SGE_END

