//
//
#include <string>
#include <iostream>

#include <argh.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <sge/common.hpp>
#include <sge/db.hpp>
#include <sge/renderer.hpp>
#include <sge/scene.hpp>
#include <sge/console.hpp>
#include <sge/input.hpp>
#include <sge/game.hpp>

SGE_BEGIN

static uv_loop_t *s_loop = NULL;
static uv_timer_t s_frame_timer;
static uv_timer_t s_state_timer;
static unsigned int s_fps = 0;
static unsigned int s_fps_count;
static uint64_t s_last_time;
static bool s_console_mode;

static inline void handle_event(const SDL_Event &event)
{
	renderer::handle_event(event);

	switch (event.type) {
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			s_console_mode = !s_console_mode;
			SDL_SetRelativeMouseMode(s_console_mode ? SDL_FALSE : SDL_TRUE);
		}
		break;
	case SDL_QUIT:
		SGE_ASSERT(s_loop != NULL);
		if (game::can_quit()) {
			uv_timer_stop(&s_frame_timer);
			uv_timer_stop(&s_state_timer);
			uv_stop(s_loop);
		}
		return;
	}

	if (s_console_mode)
		console::handle_event(event);
	else
		input::handle_event(event);
}

static inline void poll_events(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
		handle_event(event);
}

static inline void draw(void)
{
	scene::draw();

	if (s_console_mode)
		console::draw();

	// fps
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("fps", NULL, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("fps %d", s_fps);
	ImGui::End();
}

static void frame(uv_timer_t *timer)
{
	int64_t pass;
	
	pass = uv_now(s_loop) - s_last_time;
	if (pass < 0) {
		s_last_time = uv_now(s_loop);
		return;
	}

	float elapsed = float(pass) / 1000.0f;

	poll_events();

	if (!s_console_mode) {
		input::update(elapsed);
		game::update(elapsed);
		scene::update(elapsed);
	} else
		console::update(elapsed);

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
	input::init();
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
	s_console_mode = true;

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
	input::shutdown();
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
	if (db_filename.empty())
		db_filename = "default.zip";

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
