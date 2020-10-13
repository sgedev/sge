//
//
#define SDL_MAIN_HANDLED
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <memory>
#include <filesystem>

#include <argh.h>

#include <sge/vm/kernel.hpp>
#include <sge/graphics/window.hpp>
#include <sge/graphics/renderer.hpp>

struct sdl_initializer {
	int result;
	sdl_initializer(Uint32 flags): result(SDL_Init(flags)) { }
	~sdl_initializer(void) { if (result == 0) SDL_Quit(); }
	operator bool(void) const { return (result == 0); }
};

static sge::vm::kernel *kernel;
static sge::graphics::window *window;
static sge::graphics::renderer *renderer;

static void poll_events(uv_timer_t *p)
{
	SDL_Event evt;

	while (SDL_PollEvent(&evt)) {
        switch (evt.type) {
		case SDL_MOUSEMOTION:
		case SDL_KEYUP:
		case SDL_KEYDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEWHEEL:
			if (kernel != nullptr)
				kernel->post_event(evt);
			break;
		case SDL_QUIT:
            uv_stop(p->loop);
            break;
        case SDL_WINDOWEVENT:
            if (window != nullptr)
                window->handle_event(evt.window);
            break;
        default:
            break;
        }
	}

    // TEST
    if (renderer != nullptr) {
        renderer->begin();
        renderer->end();
    }
}

static void print_help(void)
{
    SGE_LOGI("usage:");
    SGE_LOGI("  sge [options] <root directory>");
    SGE_LOGI(" ");
    SGE_LOGI("options:");
    SGE_LOGI("  --init=<filename>");
    SGE_LOGI("    specified the initrc, default is '/init.lua'.");
    SGE_LOGI(" ");
    SGE_LOGI("  --debug");
    SGE_LOGI("    set log level to debug.");
    SGE_LOGI(" ");
    SGE_LOGI("  -v, --version");
    SGE_LOGI("    show version.");
    SGE_LOGI(" ");
    SGE_LOGI("  -h, --help");
    SGE_LOGI("    show this message.");
}

static void log_output(void *data, int, SDL_LogPriority priority, const char *message)
{
    FILE *fp = stdout;
    const char *prefix = "";

    if (kernel != nullptr) {
        if (std::this_thread::get_id() == kernel->thread().get_id())
            prefix = "vm: ";
    }

    if (priority == SDL_LOG_PRIORITY_ERROR ||
        priority == SDL_LOG_PRIORITY_CRITICAL ||
        priority == SDL_LOG_PRIORITY_WARN)
        fp = stderr;

    fprintf(fp, "%s%s\n", prefix, message);
}

int main(int argc, char *argv[])
{
    SDL_LogSetOutputFunction(log_output, nullptr);

#ifdef SGE_DEBUG
    SDL_LogSetPriority(SGE_LOG_CATEGORY, SDL_LOG_PRIORITY_DEBUG);
#endif

    argh::parser cmdline(argc, argv);

    if (cmdline[{ "--debug" }])
        SDL_LogSetPriority(SGE_LOG_CATEGORY, SDL_LOG_PRIORITY_DEBUG);

    if (cmdline[{ "-h", "--help" }]) {
        print_help();
        return EXIT_SUCCESS;
    }

    SGE_LOGI("sge v%d.%d.%d",
        SGE_VERSION_MAJOR, SGE_VERSION_MINOR, SGE_VERSION_PATCH);

    if (cmdline[{ "-v", "--version" }])
        return EXIT_SUCCESS;

    std::string init = "/init.lua";
    cmdline({ "--init" }) >> init;

    std::string root = std::filesystem::current_path().string();
    if (cmdline.size() > 1) {
        if (cmdline.size() != 2) {
            SGE_LOGE("too many roots.");
            return EXIT_FAILURE;
        }
        root = std::filesystem::canonical(cmdline(1).str()).string();
        if (!std::filesystem::is_directory(root)) {
            SGE_LOGE("root must be a directory.");
            return EXIT_FAILURE;
        }
    }

	sdl_initializer sdl(SDL_INIT_EVERYTHING);
	if (!sdl) {
        SGE_LOGE("failed to init SDL.");
		return EXIT_FAILURE;
	}

    std::unique_ptr<sge::vm::kernel> k(new sge::vm::kernel(uv_default_loop()));
    if (!k || !k->start(root, init)) {
        SGE_LOGE("failed to init vm.");
        return EXIT_FAILURE;
    }

    std::unique_ptr<sge::graphics::window> w(new sge::graphics::window());
    if (!w || !w->init("sge", 800, 600)) {
        SGE_LOGE("failed to init main window.");
        return EXIT_FAILURE;
    }

    std::unique_ptr<sge::graphics::renderer> r(new sge::graphics::renderer(w.get()));
    if (!r || !r->init()) {
        SGE_LOGE("failed to init renderer.");
        return EXIT_FAILURE;
    }

    kernel = k.get();
    window = w.get();
    renderer = r.get();

    uv_timer_t poll_events_timer;
	uv_timer_init(uv_default_loop(), &poll_events_timer);
    uv_timer_start(&poll_events_timer, poll_events, 0, 15);

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	uv_timer_stop(&poll_events_timer);

    renderer = nullptr;
    window = nullptr;
    kernel = nullptr;

	return EXIT_SUCCESS;
}
