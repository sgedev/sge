/*
 *
 */
#ifndef GLEX_COMMON_H
#define GLEX_COMMON_H

#include <cx/list.h>

#include <GL/glex.h>

#include <HandmadeMath.h>

#define GLEX_ASSERT CX_ASSERT
#define GLEX_INLINE CX_INLINE

void glexFatalError(const char *fmt, ...);
void glexLogPrint(GLEXLogType type, const char *fmt, va_list args);
void *glexAllocMemory(unsigned int size);
void glexFreeMemory(void *ptr);

static GLEX_INLINE void glexLogError(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	glexLogPrint(GLEX_LOG_TYPE_ERROR, fmt, args);
	va_end(args);
}

static GLEX_INLINE void glexLogWarning(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	glexLogPrint(GLEX_LOG_TYPE_WARNING, fmt, args);
	va_end(args);
}

static GLEX_INLINE void glexLogInfo(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	glexLogPrint(GLEX_LOG_TYPE_INFO, fmt, args);
	va_end(args);
}

static GLEX_INLINE void glexLogDebug(const char *fmt, ...)
{
#ifdef GLEX_DEBUG
	va_list args;

	va_start(args, fmt);
	glexLogPrint(GLEX_LOG_TYPE_DEBUG, fmt, args);
	va_end(args);
#endif
}

#endif /* GLEX_COMMON_H */

