//
//
#include <argh.h>

#include <sge/scope_guard.hpp>
#include <sge/vm/kernel.hpp>
#include <sge/boot/common.hpp>

SGE_BOOT_BEGIN

static SDL_Window *main_window;
static Uint32 main_window_id;
static bool main_window_visible;
static glm::ivec2 main_window_size;
static uv_timer_t event_poll_timer;

static void log_output_func(void *userdata, int category, SDL_LogPriority priority, const char *message)
{
	const char *prefix = nullptr;

	if (vm::is_inside())
		prefix = "vm: ";

	if (priority == SDL_LOG_PRIORITY_CRITICAL || priority == SDL_LOG_PRIORITY_ERROR || priority == SDL_LOG_PRIORITY_WARN)
		fprintf(stderr, "%s%s", prefix, message);
	else
		fprintf(stdout, "%s%s", prefix, message);
}

static void handle_main_window_event(const SDL_WindowEvent &evt)
{

}

static void handle_event(const SDL_Event &evt)
{

}

static void event_poll_cb(uv_timer_t *ptr)
{

}

static void run(void)
{
	if (!vm::start(uv_default_loop())) {
		SGE_LOGE("failed to start vm.");
		return;
	}

	scope_guard vm_guard([] { vm::stop(); });

	uv_timer_init(uv_default_loop(), &event_poll_timer);
	uv_timer_start(&event_poll_timer, &event_poll_cb, 0, 20);
	scope_guard event_poll_guard([&] { uv_timer_stop(&event_poll_timer); });

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

SGE_BOOT_END

int main(int argc, char *argv[])
{
	SDL_LogSetOutputFunction(&sge::boot::log_output_func, nullptr);

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		SGE_LOGE("failed to init SDL.");
		return EXIT_FAILURE;
	}

	sge::scope_guard sdl_guard([] { SDL_Quit(); });

	SGE_LOGI("start running...");
	sge::boot::run();
	SGE_LOGI("shuting down...");

	return EXIT_SUCCESS;
}
