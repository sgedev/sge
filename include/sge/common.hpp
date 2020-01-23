//
//
#ifndef SGE_COMMON_HPP
#define SGE_COMMON_HPP

#include <cstdarg>

#include <SDL.h>
#include <GL/glew.h>
#include <pugixml.hpp>

#include <sge/config.hpp>

#ifdef SGE_DEBUG
#	ifdef SDL_ASSERT_LEVEL
#		undef SDL_ASSERT_LEVEL
#endif
#	define SDL_ASSERT_LEVEL 3
#	include <SDL_assert.h>
#	define SGE_ASSERT(expr) SDL_assert(expr)
#else
#	define SGE_ASSERT(expr)
#endif

#define SGE_BEGIN namespace SGE {
#define SGE_END }

SGE_BEGIN

static inline void logInfo(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, fmt, args);
	va_end(args);
}

static inline void logWarning(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, fmt, args);
	va_end(args);
}

static inline void logError(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, fmt, args);
	va_end(args);
}

static inline void logDebug(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG, fmt, args);
	va_end(args);
}

SGE_END

#endif // SGE_COMMON_HPP

