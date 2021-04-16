/*
 *
 */
#ifndef SGE_COMMON_H
#define SGE_COMMON_H

#include <sge/config.h>

#ifdef SGE_DEBUG
#   define SDL_ASSERT_LEVEL 3
#   include <SDL_assert.h>
#   define SGE_ASSERT(expr) SDL_assert(expr)
#else
#   define SGE_ASSERT(expr) do { } while (0)
#endif

#include <SDL.h>
#include <uv.h>

#ifdef __cplusplus
#   define SGE_C_BEGIN extern "C" {
#   define SGE_C_END }
#else
#   define SGE_C_BEGIN
#   define SGE_C_END
#endif

#if defined(_MSC_VER)
#   define SGE_INLINE __forceinline
#   define SGE_LIKELY(x) x
#   define SGE_UNLIKELY(x) x
#elif defined(__GNUC__)
#   define SGE_INLINE inline __attribute__((always_inline))
#   define SGE_LIKELY(x) __builtin_expect(!!(x), 1)
#   define SGE_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#   error unsupported compiler.
#endif

#define SGE_UNUSED(param) ((void)(param))

#define SGE_CONCAT(a, b) SGE_CONCAT_IMPL(a, b)
#define SGE_CONCAT_IMPL(a, b) a##b

#define SGE_CONCAT3(a, b, c) SGE_CONCAT_IMPL(a, b, c)
#define SGE_CONCAT3_IMPL(a, b, c) a##b##c

#define SGE_STRIZE(x) SGE_STRIZE_IMPL(x)
#define SGE_STRIZE_IMPL(x) #x

#ifdef __COUNTER__
#   define SGE_AUTONAME(str) SGE_CONCAT(str, __COUNTER__)
#else
#   define SGE_AUTONAME(str) SGE_CONCAT(str, __LINE__)
#endif

#define SGE_PMOVB(p, offset) \
    (((unsigned char *)(p)) + (offset))

#define SGE_OFFSETOF(struct_type, member_name) \
    ((intptr_t)(&(((struct_type *)0)->member_name)))

#define SGE_MEMBEROF(p, struct_type, member_name) \
    ((struct_type *)SGE_PMOVB(p, -SGE_OFFSETOF(struct_type, member_name)))

SGE_C_BEGIN

void sge_log(SDL_LogPriority prio, const char *fmt, ...);

SGE_C_END

#define SGE_LOGI(fmt, ...) sge_log(SDL_LOG_PRIORITY_INFO, fmt, __VA_ARGS__)
#define SGE_LOGW(fmt, ...) sge_log(SDL_LOG_PRIORITY_WARN, fmt, __VA_ARGS__)
#define SGE_LOGE(fmt, ...) sge_log(SDL_LOG_PRIORITY_ERROR, fmt, __VA_ARGS__)
#define SGE_LOGD(fmt, ...) sge_log(SDL_LOG_PRIORITY_DEBUG, fmt, __VA_ARGS__)

#endif /* SGE_COMMON_H */
