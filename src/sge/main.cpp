//
//
#include <string>
#include <iostream>

#include <argh.h>
#include <physfs.h>
#include <imgui.h>
#include <imgui_dialogs.h>
#include <filesystem/path.h>
#include <filesystem/resolver.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <sge/common.hpp>
#include <sge/renderer.hpp>
#include <sge/scene.hpp>
#include <sge/game.hpp>
#include <sge/console.hpp>
#include <sge/editor.hpp>

SGE_BEGIN

enum mode {
	MODE_GAME = 0,
	MODE_CONSOLE,
	MODE_EDITOR
};

static uv_loop_t *s_loop = NULL;
static uv_timer_t s_frame_timer;
static uv_timer_t s_state_timer;
static unsigned int s_fps = 0;
static unsigned int s_fps_count;
static uint64_t s_last_time;
static filesystem::path s_game_path;
static bool s_editor_enabled;
static mode s_mode;

static inline bool handle_key_event(const SDL_Event &event)
{
	if (event.type != SDL_KEYDOWN)
		return false;

	switch (event.key.keysym.sym) {
	case SDLK_ESCAPE:
		switch (s_mode) {
		case MODE_GAME:
			s_mode = MODE_CONSOLE;
			SDL_SetRelativeMouseMode(SDL_FALSE);
			return true;
		case MODE_CONSOLE:
			s_mode = MODE_GAME;
			SDL_SetRelativeMouseMode(SDL_TRUE);
			return true;
		}
		break;
	case SDLK_F2:
		if (s_mode != MODE_EDITOR && s_editor_enabled) {
			s_mode = MODE_EDITOR;
			SDL_SetRelativeMouseMode(SDL_FALSE);
			return true;
		}
		break;
	case SDLK_F5:
		if (s_mode == MODE_EDITOR) {
			s_mode = MODE_GAME;
			SDL_SetRelativeMouseMode(SDL_TRUE);
			return true;
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

		switch (s_mode) {
		case MODE_GAME:
			game::handle_event(event);
			break;
		case MODE_CONSOLE:
			console::handle_event(event);
			break;
		case MODE_EDITOR:
			editor::handle_event(event);
			break;
		}
	}
}

static inline void draw(void)
{
	switch (s_mode) {
	case MODE_GAME:
		game::draw();
		break;
	case MODE_CONSOLE:
		console::draw();
		break;
	case MODE_EDITOR:
		editor::draw();
		break;
	}

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

	switch (s_mode) {
	case MODE_GAME:
		game::update(elapsed);
		break;
	case MODE_CONSOLE:
		console::update(elapsed);
		break;
	case MODE_EDITOR:
		editor::update(elapsed);
		break;
	}

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

static bool init(void)
{
	SGE_ASSERT(s_loop == NULL);
	s_loop = uv_default_loop();

	if (!PHYSFS_mount(s_game_path.str().c_str(), "/", 0)) {
		SGE_LOGE("failed to mount root (%d).\n", PHYSFS_getLastErrorCode());
		return false;
	}

	renderer::init();
	scene::init();
	console::init();
	game::init();

	if (s_editor_enabled) {
		editor::init();
		s_mode = MODE_EDITOR;
	} else
		s_mode = MODE_CONSOLE;

	uv_timer_init(s_loop, &s_frame_timer);
	uv_timer_init(s_loop, &s_state_timer);

	uv_timer_start(&s_frame_timer, frame, 0, 16);
	uv_timer_start(&s_state_timer, state, 1000, 1000);

	s_fps = 0;
	s_fps_count = 0;
	s_last_time = uv_now(s_loop);

	// SDL_SetRelativeMouseMode(SDL_FALSE);

	return true;
}

static void shutdown(void)
{
	SGE_LOGD("Shutdown...\n");

	uv_timer_stop(&s_frame_timer);
	uv_timer_stop(&s_state_timer);

	if (s_editor_enabled)
		editor::shutdown();

	game::shutdown();
	console::shutdown();
	scene::shutdown();
	renderer::shutdown();

	SGE_LOGI("Shutdown.\n");
}

SGE_END

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

static bool parse_cmdline(const argh::parser &cmdline)
{
	if (cmdline[{ "-v", "--version" }]) {
		show_version();
		return false;
	}

	if (cmdline[{ "-h", "--help" }]) {
		show_help();
		return false;
	}

	if (cmdline.size() != 2) {
		SGE_LOGE("No game path.\n");
		return false;
	}

	std::string game;
	cmdline(1) >> game;

	filesystem::path path = game;
	path = path.make_absolute();
	if (!path.exists()) {
		SGE_LOGD("Invalid game path.\n");
		return false;
	}

	SGE_LOGI("Game path: '%s'\n", path.str().c_str());

	if (cmdline[{ "-e", "--edit" }]) {
		if (!path.is_directory()) {
			SGE_LOGE("Editor should not works with archive.\n");
			return false;
		}
		sge::s_editor_enabled = true;
	}

	sge::s_game_path = path;

	return true;
}

int main(int argc, char *argv[])
{
	int ret = EXIT_FAILURE;
	argh::parser cmdline(argv);

	SDL_SetMainReady();

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		goto end0;

#ifdef SGE_DEBUG
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

	if (!PHYSFS_init(NULL))
		goto end1;
	
	if (!parse_cmdline(cmdline))
		goto end2;

	if (!sge::init())
		goto end2;

	SGE_LOGI("Running...\n");
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	ret = EXIT_SUCCESS;

	sge::shutdown();

end2:
	PHYSFS_deinit();

end1:
	SDL_Quit();

end0:
	SGE_LOGI("Quit.\n");

	return ret;
}

