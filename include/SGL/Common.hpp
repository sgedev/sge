//
//
#ifndef SGL_COMMON_HPP
#define SGL_COMMON_HPP

#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <SGL/Config.hpp>

#ifdef SGL_DEBUG
#   include <SDL_assert.h>
#   define SDL_ASSERT_LEVEL 2
#   define SGL_ASSERT(expr) SDL_assert(expr)
#else
#   define SGL_ASSERT(expr)
#endif

#ifdef SGL_LOG
#	define SGL_LOG_CATEGORY SDL_LOG_CATEGORY_CUSTOM
#	define SGL_LOGE(fmt, ...) SDL_LogError(SGL_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#	define SGL_LOGW(fmt, ...) SDL_LogWarn(SGL_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#	define SGL_LOGI(fmt, ...) SDL_LogInfo(SGL_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#	define SGL_LOGD(fmt, ...) SDL_LogDebug(SGL_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#else
#	define SGL_LOGE(fmt, ...) do { } while (0)
#	define SGL_LOGW(fmt, ...) do { } while (0)
#	define SGL_LOGI(fmt, ...) do { } while (0)
#	define SGL_LOGD(fmt, ...) do { } while (0)
#endif

#define SGL_BEGIN namespace SGL {
#define SGL_END }

#endif // SGL_COMMON_HPP

