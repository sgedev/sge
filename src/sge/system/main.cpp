//
//
#include <string>
#include <memory>
#include <filesystem>

#include <lua.hpp>
#if 0
#include <argh.h>
#include <physfs.h>
#include <spdlog/spdlog.h>

#include <sge/scopeguard.hpp>
#include <sge/system/common.hpp>
#include <sge/system/base.hpp>
#include <sge/system/local.hpp>
#include <sge/system/client.hpp>
#include <sge/system/server.hpp>

SGE_SYSTEM_BEGIN

struct Settings {
    enum class Mode {
        Local = 0,
        Client,
        Server,
        Editor
    };

    bool debug = false;
    bool verbose = false;
    std::filesystem::path rootfs = std::filesystem::current_path();
    Mode mode = Mode::Local;

    Settings(const argh::parser& cmdline) {
        if (cmdline[{ "-d", "--debug" }]) {
            debug = true;
            spdlog::set_level(spdlog::level::debug);
        } else {
            spdlog::set_level(spdlog::level::info);
        }

        if (cmdline[{ "--verbose" }]) {
            verbose = true;
            spdlog::set_level(spdlog::level::trace);
        }

        if (cmdline.size() > 0) {
            if (cmdline.size() != 1) {
                throw std::runtime_error("Too many roofs.");
            }
            rootfs = std::filesystem::canonical(cmdline[0]);
        }

        std::string mode_name;
        cmdline({ "-m", "--mode" }, "local") >> mode_name;
        if (mode_name == "local" || mode_name == "Local") {
            mode = Mode::Local;
        }
    }

    const char* modeName() const noexcept {
        const char* p;
        switch (mode) {
        case Mode::Local:
            p = "Local";
            break;
        case Mode::Client:
            p = "Client";
            break;
        case Mode::Server:
            p = "Server";
            break;
        case Mode::Editor:
            p = "Editor";
            break;
        default:
            p = "Default";
            break;
        }
        return p;
    }
};

static int run(const Settings& settings) {
    logInfo("SGE - v{}, debug {}, verbose {}", SGE_VERSION_STR, settings.debug, settings.verbose);
    logInfo("Mode: {}", settings.modeName());
    logInfo("RootFS: {}", settings.rootfs);

    int ret = PHYSFS_init("sge");
    if (!ret) {
        throw std::runtime_error("Failed to init PhysFS.");
    }
    auto physfs_guard = scopeGuard([] {
        PHYSFS_deinit();
    });

    ret = PHYSFS_mount(settings.rootfs.string().c_str(), "/", 1);
    if (!ret) {
        throw std::runtime_error(std::format("Failed to mount root fs: {}", settings.rootfs));
    }

    std::unique_ptr<sge::system::Base> app;
    switch (settings.mode) {
    case Settings::Mode::Client:
        app = std::make_unique<sge::system::Client>();
        break;
    case Settings::Mode::Server:
        app = std::make_unique<sge::system::Server>();
        break;
    case Settings::Mode::Editor:
        //app = std::make_unique<sge::system::Editor>();
        break;
    default:
        app = std::make_unique<sge::system::Local>();
        break;
    }

    app->run();
}

SGE_SYSTEM_END
#endif

int main(int argc, char* argv[]) {
#if 0
    sge::system::Settings settings(argh::parser(argv));

    int exit_code = EXIT_FAILURE;
    try {
        exit_code = sge::system::run(settings);
    } catch (const std::exception& e) {
        spdlog::error("Exception: {}", e.what());
    } catch (...) {
        spdlog::error("Unknwon exception.");
    }

    return exit_code;
#else
    return 0;
#endif
}
