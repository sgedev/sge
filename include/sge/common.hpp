//
//
#ifndef SGE_COMMON_HPP
#define SGE_COMMON_HPP

#include <stdexcept>

#include <spdlog/spdlog.h>

#include <sge/common.h>

#define SGE_BEGIN namespace sge {
#define SGE_END }

SGE_BEGIN

template <typename... Args>
void logError(spdlog::format_string_t<Args...> fmt, Args... args) {
    spdlog::log(spdlog::level::err, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void logWarn(spdlog::format_string_t<Args...> fmt, Args... args) {
    spdlog::log(spdlog::level::warn, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void logInfo(spdlog::format_string_t<Args...> fmt, Args... args) {
    spdlog::log(spdlog::level::info, fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void logDebug(spdlog::format_string_t<Args...> fmt, Args... args) {
    spdlog::log(spdlog::level::debug, fmt, std::forward<Args>(args)...);
}

SGE_END

#endif // SGE_COMMON_HPP
