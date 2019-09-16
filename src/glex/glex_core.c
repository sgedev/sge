/*
 *
 */
#include <stdio.h>
#include <stdlib.h>

#include "glex_common.h"

GLEXContext *glex;

static GLEXSystem glexSys;

static inline const char *glexDebugSource(GLenum source)
{
	switch (source) {
	case GL_DEBUG_SOURCE_API:             return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "WINDOW_SYSTEM";
	case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER_COMPILER";
	case GL_DEBUG_SOURCE_THIRD_PARTY:     return "THIRD_PART";
	case GL_DEBUG_SOURCE_APPLICATION:     return "APPLICATION";
	case GL_DEBUG_SOURCE_OTHER:           return "OTHER";
	}
	return "";
}

static inline const char *glexDebugType(GLenum type)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:               return "ERROR";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "UNDEFINED_BEHAVIOR";
	case GL_DEBUG_TYPE_PORTABILITY:         return "PORTABILITY";
	case GL_DEBUG_TYPE_PERFORMANCE:         return "PERFORMANCE";
	case GL_DEBUG_TYPE_MARKER:              return "MARKER";
	case GL_DEBUG_TYPE_PUSH_GROUP:          return "PUSH_GROUP";
	case GL_DEBUG_TYPE_POP_GROUP:           return "POP_GROUP";
	case GL_DEBUG_TYPE_OTHER:               return "OTHER";
	}
	return "";
}

static inline const char *glexDebugSeverity(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:         return "HIGH";
	case GL_DEBUG_SEVERITY_MEDIUM:       return "MEDIUM";
	case GL_DEBUG_SEVERITY_LOW:          return "LOW";
	case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
	}
	return "";
}

static void glexDebugOutput(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar* message, const void *data)
{
	if (type == GL_DEBUG_TYPE_ERROR)
		glexLogError("GL-%04d|%s|%s|%s] %s\n", id, glexDebugSource(source), glexDebugType(type), glexDebugSeverity(severity), message);
	else
		glexLogDebug("GL-%04d|%s|%s|%s] %s\n", id, glexDebugSource(source), glexDebugType(type), glexDebugSeverity(severity), message);
}

static void glexSysFatalErrorDefault(const char *fmt, va_list args)
{
	vprintf(fmt, args);
	exit(EXIT_FAILURE);
}

static void glexSysLogPrintDefault(GLEXLogType type, const char *fmt, va_list args)
{
	vprintf(fmt, args);
}

static void* glexSysAllocMemoryDefault(unsigned int size)
{
	if (size < 1)
		return NULL;

	return malloc(size);
}

static void glexSysFreeMemoryDefault(void* ptr)
{
	if (ptr != NULL)
		free(ptr);
}

void glexFatalError(const char *fmt, ...)
{
	va_list args;

	GLEX_ASSERT(glexSys.FatalError != NULL);

	va_start(args, fmt);
	glexSys.FatalError(fmt, args);
	va_end(args);
}

void glexLogPrint(GLEXLogType type, const char* fmt, va_list args)
{
	GLEX_ASSERT(glexSys.LogPrint != NULL);

	glexSys.LogPrint(type, fmt, args);
}

void *glexAllocMemory(unsigned int size)
{
	GLEX_ASSERT(glexSys.AllocMemory != NULL);

	return glexSys.AllocMemory(size);
}

void glexFreeMemory(void *ptr)
{
	GLEX_ASSERT(glexSys.FreeMemory != NULL);

	glexSys.FreeMemory(ptr);
}

GLEX_API int glexInit(const GLEXSystem *system)
{
	glexSys.LogPrint = glexSysLogPrintDefault;
	glexSys.FatalError = glexSysFatalErrorDefault;
	glexSys.AllocMemory = glexSysAllocMemoryDefault;
	glexSys.FreeMemory = glexSysFreeMemoryDefault;

	if (system == NULL)
		return 0;

	if (system->AllocMemory != NULL && system->FreeMemory == NULL)
		return -1;

	if (system->AllocMemory == NULL && system->FreeMemory != NULL)
		return -1;

	if (system->AllocMemory != NULL && system->FreeMemory != NULL) {
		glexSys.AllocMemory = system->AllocMemory;
		glexSys.FreeMemory = system->FreeMemory;
	}

	if (system->LogPrint != NULL)
		glexSys.LogPrint = system->LogPrint;

	if (system->FatalError != NULL)
		glexSys.FatalError = system->FatalError;

	return 0;
}

GLEX_API void glexShutdown(void)
{
}

GLEX_API GLEXContext *glexCreateContext(const GLEXConfig *config)
{
	GLEXContext *context;

	context = glexAllocMemory(sizeof(GLEXContext));
	if (context == NULL)
		goto bad0;

	if (glxwInitCtx(&context->glxw_ctx) < 0)
		goto bad1;

	if (GL_KHR_debug)
		glDebugMessageCallback(glexDebugOutput, NULL);

	if (!glexInitProgram(context)) {
		glexLogError("Failed to initialize program.\n");
		goto bad2;
	}

	if (!glexInitMaterial(context)) {
		glexLogError("Failed to initialize material.\n");
		goto bad2;
	}

	if (!glexInitLight(context)) {
		glexLogError("Failed to initialize light.\n");
		goto bad3;
	}

	if (!glexInitMesh(context)) {
		glexLogError("Failed to initialize mesh.\n");
		goto bad4;
	}

	if (!glexInitTest(context)) {
		glexLogError("Failed to initialize test.\n");
		goto bad5;
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return 0;

bad5:
	glexShutdownMesh(context);

bad4:
	glexShutdownLight(context);

bad3:
	glexShutdownMaterial(context);

bad2:
	glexShutdownProgram(context);

bad1:
	glexFreeMemory(context);

bad0:
	return -1;
}

GLEX_API void glexDeleteContext(GLEXContext *context)
{
	if (context == NULL)
		return;

	if (context == glex)
		glexMakeCurrent(NULL);

	glexShutdownTest(context);
	glexShutdownMesh(context);
	glexShutdownLight(context);
	glexShutdownMaterial(context);
	glexShutdownProgram(context);
}

GLEX_API GLEXContext *glexGetCurrentContext(void)
{
	return glex;
}

GLEX_API void glexMakeCurrent(GLEXContext *context)
{
	glex = context;

	if (glex != NULL)
		glxw = &glex->glxw_ctx;
	else
		glxw = NULL;
}

