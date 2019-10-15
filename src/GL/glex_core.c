/*
 *
 */
#include <stdio.h>
#include <stdlib.h>

#include "glex_common.h"
#include "glex_context.h"

static GLEXSystem glexSystem;
static GLEXLogLevel glexCurrentLogLevel;

static void glexDefaultLogWrite(GLEXLogLevel level, const char *fmt, va_list args)
{
	vprintf(fmt, args);
}

static void *glexDefaultAllocMemory(unsigned int size)
{
	return malloc(size);
}

static void glexDefaultFreeMemory(void *ptr)
{
	free(ptr);
}

GLEX_API int glexInit(const GLEXSystem *system)
{
	glex = NULL;

	glexSystem.logWrite = glexDefaultLogWrite;
	glexSystem.allocMemory = glexDefaultAllocMemory;
	glexSystem.freeMemory = glexDefaultFreeMemory;

#ifdef GLEX_DEBUG
	glexCurrentLogLevel = GLEX_LOG_DEBUG;
#else
	glexCurrentLogLevel = GLEX_LOG_WARNING;
#endif

	glexInitContextGlobals();

	if (system == NULL)
		return GLEX_OK;

	if (((!!system->allocMemory) + (!!system->freeMemory)) != 2)
		return GLEX_INVALID_PARAM;

	glexSystem.logWrite = system->logWrite;
	glexSystem.allocMemory = system->allocMemory;
	glexSystem.freeMemory = system->freeMemory;

	return GLEX_OK;
}

GLEX_API void glexShutdown(void)
{
	if (glex != NULL) {
		glexDeleteContext(glex);
		glex = NULL;
	}
}

GLEX_API void glexLogLevel(GLEXLogLevel level)
{
	glexCurrentLogLevel = level;
}

GLEX_API GLEXLogLevel glexGetLogLevel(void)
{
	return glexCurrentLogLevel;
}

void glexLogWrite(GLEXLogLevel level, const char *fmt, va_list args)
{
	if (level < glexCurrentLogLevel)
		return;

	GLEX_ASSERT(glexSystem.logWrite != NULL);
	glexSystem.logWrite(level, fmt, args);
}

void *glexAllocMemory(GLuint size)
{
	GLEX_ASSERT(glexSystem.allocMemory != NULL);
	return glexSystem.allocMemory(size);
}

void glexFreeMemory(void *ptr)
{
	GLEX_ASSERT(glexSystem.freeMemory != NULL);
	return glexSystem.freeMemory(ptr);
}

#define HANDMADE_MATH_IMPLEMENTATION
#include "HandmadeMath.h"

