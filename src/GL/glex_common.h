/*
 *
 */
#ifndef GLEX_COMMON_H
#define GLEX_COMMON_H

#include <cx/common.h>

#include <GL/gl3w.h>
#include <GL/glex.h>

#include "HandmadeMath.h"

#define GLEX_ASSERT CX_ASSERT
#define GLEX_INLINE inline

void glexLogWrite(GLEXLogLevel level, const char *fmt, va_list args);
void *glexAllocMemory(GLuint size);
void glexFreeMemory(void *ptr);

static inline void glexLogError(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	glexLogWrite(GLEX_LOG_ERROR, fmt, args);
	va_end(args);
}

static inline void glexLogWarning(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	glexLogWrite(GLEX_LOG_WARNING, fmt, args);
	va_end(args);
}

static inline void glexLogInfo(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	glexLogWrite(GLEX_LOG_INFO, fmt, args);
	va_end(args);
}

static inline void glexLogDebug(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	glexLogWrite(GLEX_LOG_DEBUG, fmt, args);
	va_end(args);
}

#endif /* GLEX_COMMON_H */

