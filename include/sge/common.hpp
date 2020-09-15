//
//
#ifndef SGE_COMMON_HPP
#define SGE_COMMON_HPP

#include <sge/config.hpp>

#include <uv.h>

#ifdef SGE_DEBUG
#	define SDL_ASSERT_LEVEL 2
#	include <SDL_assert.h>
#	define SGE_ASSERT(expr)
#else
#	define SGE_ASSERT(expr)
#endif

#ifndef SGE_INLINE
#	define SGE_INLINE
#endif

#define SGE_LOGE(fmt, ...) SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, fmt, ##__VA_ARGS__)
#define SGE_LOGW(fmt, ...) SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, fmt, ##__VA_ARGS__)
#define SGE_LOGI(fmt, ...) SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, fmt, ##__VA_ARGS__)

#ifdef SGE_DEBUG
#	define SGE_LOGD(fmt, ...) SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG, fmt, ##__VA_ARGS__)
#else
#	define SGE_LOGD(fmt, ...)
#endif

#include <SDL.h>

#define SGE_LOG

#define SGE_BEGIN namespace sge {
#define SGE_END }

#endif // SGE_COMMON_HPP
