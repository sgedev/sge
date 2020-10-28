/*
 *
 */
#ifndef SGE_COMMON_H
#define SGE_COMMON_H

#include <stdbool.h>

#include <sge/config.h>

#ifdef SGE_DEBUG
#    include <SDL_assert.h>
#    define SDL_ASSERT_LEVEL 2
#    define SGE_ASSERT(expr) SDL_assert(expr)
#else
#    define SGE_ASSERT(expr)
#endif

#include <uv.h>
#include <SDL.h>

#ifdef __cplusplus
#    define SGE_BEGIN_C_DECLS extern "C" {
#    define SGE_END_C_DECLS }
#else
#    define SGE_BEGIN_C_DECLS
#    define SGE_END_C_DECLS
#endif

#if defined(_MSC_VER)
#    define SGE_LIKELY(x) (x)
#    define SGE_UNLIKELY(x) (x)
#    define SGE_FORCE_INLINE __forceinline
#    define SGE_PRINTF_LIKE(a, b)
#    define SGE_INT64 __int64
#    define SGE_UINT64 unsigned __int64
#elif defined(__GNUC__)
#    define SGE_LIKELY(x) __builtin_expect(!!(x), 1)
#    define SGE_UNLIKELY(x) __builtin_expect(!!(x), 0)
#    define SGE_FORCE_INLINE inline __attribute__((always_inline))
#    define SGE_PRINTF_LIKE(f, a) __attribute__((format(printf, f, a)))
#    ifdef _LP64
#        define SGE_INT64 long
#        define SGE_UINT64 unsigned long
#    else
#        define SGE_INT64 long long
#        define SGE_UINT64 unsgined long long
#    endif
#else
#    error unsupported compiler.
#endif

#if defined(_WIN32)
#    define SGE_OS_WINDOWS
#elif defined(__linux__)
#    define SGE_OS_LINUX
#else
#    error unknown os.
#endif

#define SGE_UNUSED(x) ((void)(x))

#define SGE_INLINE SGE_FORCE_INLINE

#define SGE_CONCAT(a, b) SGE_CONCAT_IMPL(a, b)
#define SGE_CONCAT_IMPL(a, b) a##b

#define SGE_CONCAT3(a, b, c) SGE_CONCAT_IMPL(a, b, c)
#define SGE_CONCAT3_IMPL(a, b, c) a##b##c

#define SGE_STRIZE(x) SGE_STRIZE_IMPL(x)
#define SGE_STRIZE_IMPL(x) #x

#define SGE_PMOVB(p, offset) \
    (((unsigned char *)(p)) + (offset))

#define SGE_OFFSETOF(struct_type, member_name) \
    ((intptr_t)(&(((struct_type *)0)->member_name)))

#define SGE_MEMBEROF(p, struct_type, member_name) \
    ((struct_type *)SGE_PMOVB(p, -SGE_OFFSETOF(struct_type, member_name)))

#define SGE_LOG_CATEGORY SDL_LOG_CATEGORY_APPLICATION

#define SGE_LOGE(fmt, ...) SDL_LogMessage(SGE_LOG_CATEGORY, SDL_LOG_PRIORITY_ERROR, fmt, ##__VA_ARGS__)
#define SGE_LOGW(fmt, ...) SDL_LogMessage(SGE_LOG_CATEGORY, SDL_LOG_PRIORITY_WARN, fmt, ##__VA_ARGS__)
#define SGE_LOGI(fmt, ...) SDL_LogMessage(SGE_LOG_CATEGORY, SDL_LOG_PRIORITY_INFO, fmt, ##__VA_ARGS__)

#ifdef SGE_DEBUG
#    define SGE_LOGD(fmt, ...) SDL_LogMessage(SGE_LOG_CATEGORY, SDL_LOG_PRIORITY_DEBUG, fmt, ##__VA_ARGS__)
#else
#    define SGE_LOGD(fmt, ...)
#endif

#endif /* SGE_H */
