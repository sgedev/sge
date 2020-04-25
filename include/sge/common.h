/*
 *
 */
#ifndef SGE_COMMON_H
#define SGE_COMMON_H

#include <stdbool.h>

#include <cx/common.h>

#include <HandmadeMath.h>

#include <GL/glew.h>
#include <GL/glex.h>

#include <nanovg.h>

#include <SDL.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <physfs.h>

#include <sge/config.h>

#ifdef SGE_DEBUG
#   include <SDL_assert.h>
#   define SDL_ASSERT_LEVEL 2
#   define SGE_ASSERT(expr) SDL_assert(expr)
#else
#   define SGE_ASSERT(expr)
#endif

#ifdef SGE_LOG
#	define SGE_LOG_CATEGORY SDL_LOG_CATEGORY_CUSTOM
#	define SGE_LOGE(fmt, ...) SDL_LogError(SGE_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#	define SGE_LOGW(fmt, ...) SDL_LogWarn(SGE_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#	define SGE_LOGI(fmt, ...) SDL_LogInfo(SGE_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#	define SGE_LOGD(fmt, ...) SDL_LogDebug(SGE_LOG_CATEGORY, fmt, ##__VA_ARGS__)
#else
#	define SGE_LOGE(fmt, ...) do { } while (0)
#	define SGE_LOGW(fmt, ...) do { } while (0)
#	define SGE_LOGI(fmt, ...) do { } while (0)
#	define SGE_LOGD(fmt, ...) do { } while (0)
#endif

#endif /* SGE_COMMON_H */
