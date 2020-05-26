/*
 *
 */
#ifndef SGE_COMMON_H
#define SGE_COMMON_H

#include <stdio.h>

#include <cx/common.h>
#include <cx/math.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <sge/config.h>

#define SGE_BEGIN_C_DECLS CX_BEGIN_C_DECLS
#define SGE_END_C_DECLS CX_END_C_DECLS

#define SGE_INLINE CX_INLINE

#ifdef SGE_DEBUG
#   include <SDL_assert.h>
#	ifdef SDL_ASSERT_LEVEL
#		undef SDL_ASSERT_LEVEL
#	endif
#   define SDL_ASSERT_LEVEL 2
#   define SGE_ASSERT(expr) SDL_assert(expr)
#else
#   define SGE_ASSERT(expr)
#endif

#ifdef SGE_LOG
#	define SGE_LOGE(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#	define SGE_LOGW(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#	define SGE_LOGI(fmt, ...) fprintf(stdout, fmt, ##__VA_ARGS__)
#	define SGE_LOGD(fmt, ...) fprintf(stdout, fmt, ##__VA_ARGS__)
#else
#	define SGE_LOGE(fmt, ...) do { } while (0)
#	define SGE_LOGW(fmt, ...) do { } while (0)
#	define SGE_LOGI(fmt, ...) do { } while (0)
#	define SGE_LOGD(fmt, ...) do { } while (0)
#endif

#endif /* SGE_COMMON_H */
