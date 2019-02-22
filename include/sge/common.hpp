//
//
#ifndef SGL_COMMON_H
#define SGL_COMMON_H

#include <Eigen/Geometry>

#include <SGL.h>

#ifdef SGL_DEBUG
#	undef SDL_ASSERT_LEVEL
#	define SDL_ASSERT_LEVEL 2
#	include <SDL_assert.h>
#	define SGL_ASSERT(expr) SDL_assert(expr)
#	define SGL_LOG_CATEGORY SDL_LOG_CATEGORY_APPLICATION
#	ifdef __GNUC__
#		define SGL_LOGI(fmt, ...) SDL_LogInfo(SGL_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#		define SGL_LOGE(fmt, ...) SDL_LogError(SGL_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#		define SGL_LOGW(fmt, ...) SDL_LogWarn(SGL_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#		define SGL_LOGD(fmt, ...) SDL_LogDebug(SGL_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#	else
#		define SGL_LOGI(fmt, ...) SDL_LogInfo(SGL_LOG_CATEGORY, fmt, __VA_ARGS__)
#		define SGL_LOGE(fmt, ...) SDL_LogError(SGL_LOG_CATEGORY, fmt, __VA_ARGS__)
#		define SGL_LOGW(fmt, ...) SDL_LogWarn(SGL_LOG_CATEGORY, fmt, __VA_ARGS__)
#		define SGL_LOGD(fmt, ...) SDL_LogDebug(SGL_LOG_CATEGORY, fmt, __VA_ARGS__)
#	endif
#else
#	define SGL_ASSERT(expr)
#	define SGL_LOGI(fmt, ...)
#	define SGL_LOGE(fmt, ...)
#	define SGL_LOGW(fmt, ...)
#	define SGL_LOGD(fmt, ...)
#endif

#define SGL_CONCAT(a, b) SGL_CONCAT_IMPL(a, b)
#define SGL_CONCAT_IMPL(a, b) a##b

#define SGL_STRIZE(x) SGL_STRIZE_IMPL(x)
#define SGL_STRIZE_IMPL(x) #x

#define SGL_BEGIN namespace SGL {
#define SGL_END }

SGL_BEGIN

SGL_END

#endif // SGL_COMMON_H
