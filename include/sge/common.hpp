//
//
#ifndef SGE_COMMON_HPP
#define SGE_COMMON_HPP

#include <cstdint>

#include <glm.hpp>

#include <uv.h>
#include <SDL.h>
#include <physfs.h>

#include <sge/config.hpp>

#ifdef SGE_DEBUG
#	undef SDL_ASSERT_LEVEL
#	define SDL_ASSERT_LEVEL 2
#	include <SDL_assert.h>
#	define SGE_ASSERT(expr) SDL_assert(expr)
#else
#	define SGE_ASSERT(expr)
#endif

#define SGE_LOG_CATEGORY SDL_LOG_CATEGORY_APPLICATION

#ifdef __GNUC__
#	define SGE_LOGI(fmt, ...) SDL_LogInfo(SGE_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#	define SGE_LOGE(fmt, ...) SDL_LogError(SGE_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#	define SGE_LOGW(fmt, ...) SDL_LogWarn(SGE_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#	define SGE_LOGD(fmt, ...) SDL_LogDebug(SGE_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#else
#	define SGE_LOGI(fmt, ...) SDL_LogInfo(SGE_LOG_CATEGORY, fmt, __VA_ARGS__)
#	define SGE_LOGE(fmt, ...) SDL_LogError(SGE_LOG_CATEGORY, fmt, __VA_ARGS__)
#	define SGE_LOGW(fmt, ...) SDL_LogWarn(SGE_LOG_CATEGORY, fmt, __VA_ARGS__)
#	define SGE_LOGD(fmt, ...) SDL_LogDebug(SGE_LOG_CATEGORY, fmt, __VA_ARGS__)
#endif

#define SGE_BEGIN namespace sge {
#define SGE_END }

#endif // SGE_COMMON_HPP

