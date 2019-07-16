//
//
#ifndef SGE_COMMON_H
#define SGE_COMMON_H

#include <cstdint>

#include <uv.h>

#include <glm.hpp>

#define SDL_MAIN_HANDLED
#include <SDL.h>

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

#define SGE_CONCAT(a, b) SGE_CONCAT_IMPL(a, b)
#define SGE_CONCAT_IMPL(a, b) a##b

#define SGE_STRIZE(x) SGE_STRIZE_IMPL(x)
#define SGE_STRIZE_IMPL(x) #x

#define SGE_PMOVB(p, offset) \
	(((unsigned char *)(p)) + (offset))

#define SGE_OFFSETOF(struct_type, member_name) \
	((intptr_t)(&(((struct_type *)0)->member_name)))

#define SGE_MEMBEROF(p, struct_type, member_name) \
	((struct_type *)SGE_PMOVB(p, -SGE_OFFSETOF(struct_type, member_name)))

#define SGE_BEGIN namespace sge {
#define SGE_END }

SGE_BEGIN

typedef std::int8_t int8_t;
typedef std::uint8_t uint8_t;
typedef std::int16_t int16_t;
typedef std::uint16_t uint16_t;
typedef std::int32_t int32_t;
typedef std::uint32_t uint32_t;
typedef std::int64_t int64_t;
typedef std::uint64_t uint64_t;

SGE_END

#endif // SGE_COMMON_H
