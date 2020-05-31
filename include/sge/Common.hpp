//
//
#ifndef SGE_COMMON_HPP
#define SGE_COMMON_HPP

#include <cstdio>
#include <cstdarg>
#include <cstddef>

#include <uv.h>
#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <SGE/Config.hpp>

#define SGE_INLINE inline

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

#define SGE_BEGIN namespace SGE {
#define SGE_END }

SGE_BEGIN

enum Log {
	LogError = 0,
	LogWarning,
	LogInfo,
	LogDebug
};

void logWrite(Log type, const char *fmt, va_list args);

SGE_INLINE void logError(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	logWrite(LogError, fmt, args);
	va_end(args);
}

SGE_INLINE void logWarning(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	logWrite(LogWarning, fmt, args);
	va_end(args);
}

SGE_INLINE void logInfo(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	logWrite(LogInfo, fmt, args);
	va_end(args);
}

SGE_INLINE void logDebug(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	logWrite(LogDebug, fmt, args);
	va_end(args);
}

SGE_END

#endif // SGE_COMMON_HPP
