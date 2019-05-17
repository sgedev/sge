//
//
#include <string>
#include <iostream>

#include <argh.h>

#include <imgui.h>
#include <imgui_utils.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <sge/common.hpp>
#include <sge/db.hpp>
#include <sge/renderer.hpp>
#include <sge/scene.hpp>
#include <sge/game.hpp>
#include <sge/console.hpp>

SGE_BEGIN

static uv_loop_t *s_loop = NULL;
static uv_timer_t s_frame_timer;
static uv_timer_t s_state_timer;
static unsigned int s_fps = 0;
static unsigned int s_fps_count;
static uint64_t s_last_time;
static bool s_console_enabled;

static inline bool handle_key_event(const SDL_Event &event)
{
	if (event.type != SDL_KEYDOWN)
		return false;

	switch (event.key.keysym.sym) {
	case SDLK_ESCAPE:
		if (!s_console_enabled) {
			s_console_enabled = true;
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}
		break;
	case SDLK_F5:
		if (s_console_enabled) {
			s_console_enabled = false;
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}
		break;
	}

	return false;
}

static inline void poll_events(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		handle_key_event(event);

		if (renderer::handle_event(event))
			continue;

		if (s_console_enabled)
			console::handle_event(event);
		else
			game::handle_event(event);
	}
}

static inline void draw(void)
{
	if (s_console_enabled)
		console::draw();

	game::draw();

	ImGui::ShowDemoWindow(NULL);

	glm::ivec2 size = renderer::window_size();
	ImGui::SetNextWindowPos(ImVec2(0, size.y - 24));
	ImGui::Begin("fps", NULL, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);

	ImVec4 fps_color;

	if (s_fps > 59)
		fps_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	else if (s_fps < 24)
		fps_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	else
		fps_color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);

	ImGui::TextColored(fps_color, "fps %d", s_fps);
	ImGui::End();

	static bool v = true;
	if (v) {
		ImGui::Utils::Result res = ImGui::Utils::MessageBox("test", ImGui::Utils::MessageBoxType_ok, "test %d", s_fps);
		switch (res) {
		case ImGui::Utils::Result_ok:
			v = false;
			break;
		}
	} else {
		static bool v = true;
		if (v) {
			std::string path;
			ImGui::Utils::Result res = ImGui::Utils::OpenFileDialog("New", path);
			if (res == ImGui::Utils::Result_ok) {
				SGE_LOGD("path '%s'\n", path.c_str());
				v = false;
			}
		}
	}
}

static void frame(uv_timer_t *timer)
{
	int64_t pass = uv_now(s_loop) - s_last_time;
	if (pass < 0) {
		s_last_time = uv_now(s_loop);
		return;
	}

	float elapsed = float(pass) / 1000.0f;

	poll_events();

	if (s_console_enabled)
		console::update(elapsed);
	else
		game::update(elapsed);

	if (renderer::begin()) {
		draw();
		renderer::end();
	}

	uv_update_time(s_loop);
	s_last_time = uv_now(s_loop);
	s_fps_count++;
}

static void state(uv_timer_t *timer)
{
	s_fps = s_fps_count;
	s_fps_count = 0;
}

static bool init(uv_loop_t *loop, const char *db_filename)
{
	SGE_ASSERT(s_loop == NULL);
	SGE_ASSERT(loop != NULL);
	SGE_ASSERT(db_filename != NULL);

	s_loop = loop;

	db::init(db_filename);
	renderer::init();
	scene::init();
	console::init();
	game::init();

	uv_timer_init(loop, &s_frame_timer);
	uv_timer_init(loop, &s_state_timer);

	uv_timer_start(&s_frame_timer, frame, 0, 16);
	uv_timer_start(&s_state_timer, state, 1000, 1000);

	s_fps = 0;
	s_fps_count = 0;
	s_last_time = uv_now(s_loop);
	s_console_enabled = true;

	SDL_SetRelativeMouseMode(SDL_FALSE);

	return true;
}

static void shutdown(void)
{
	SGE_LOGD("Shutdown...\n");

	uv_timer_stop(&s_frame_timer);
	uv_timer_stop(&s_state_timer);
	SGE_LOGD("\n");

	game::shutdown();
	SGE_LOGD("\n");
	console::shutdown();
	SGE_LOGD("\n");
	scene::shutdown();
	renderer::shutdown();
	//db::shutdown();

	SGE_LOGI("Shutdown.\n");
}

SGE_END

struct SDL_Session {
	int InitResult;

	SDL_Session(void)
	{
		InitResult = SDL_Init(SDL_INIT_EVERYTHING);
	}

	~SDL_Session(void)
	{
		if (InitResult == 0)
			SDL_Quit();
	}

	operator bool(void) const
	{
		return (InitResult == 0);
	}
};

static void show_version(void)
{
	const char *type =
#ifdef SGE_DEBUG
		"DEBUG";
#else
		"RELEASE";
#endif

	SGE_LOGI("SGE: %d.%d.%d [%s]\n",
		SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH, type);

	SGE_LOGI("Platform: %s, CPUs %d, Memory %dMB\n",
		SDL_GetPlatform(), SDL_GetCPUCount(), SDL_GetSystemRAM());
}

static void show_help(void)
{
}

int main(int argc, char *argv[])
{
	argh::parser cmdline(argv);
	
	if (cmdline[{ "-v", "--version" }]) {
		show_version();
		return EXIT_SUCCESS;
	}

	if (cmdline[{ "-h", "--help" }]) {
		show_help();
		return EXIT_SUCCESS;
	}

	std::string db_filename;
	cmdline(1) >> db_filename;
	if (db_filename.empty()) {
	}

	SDL_SetMainReady();

	SDL_Session SDL;
	if (!SDL)
		return EXIT_FAILURE;

#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

	if (!sge::init(uv_default_loop(), db_filename.c_str()))
		return EXIT_FAILURE;

	SGE_LOGI("Running...\n");
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	sge::shutdown();

	SGE_LOGI("Quit.\n");

	return EXIT_SUCCESS;
}

