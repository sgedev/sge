/*
 *
 */
#include <stdio.h>
#include <stdlib.h>

#include "glex_common.h"
#include "glex_program.h"
#include "glex_material.h"
#include "glex_light.h"
#include "glex_mesh.h"
#include "glex_test.h"

static GLEXConfig glexConfig;

static inline const char* glexDebugSource(GLenum source)
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

static inline const char* glexDebugType(GLenum type)
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

static inline const char* glexDebugSeverity(GLenum severity)
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
	GLenum severity, GLsizei length, const GLchar* message, const void* data)
{
	if (type == GL_DEBUG_TYPE_ERROR)
		glexLogError("GL-%04d|%s|%s|%s] %s\n", id, glexDebugSource(source), glexDebugType(type), glexDebugSeverity(severity), message);
	else
		glexLogDebug("GL-%04d|%s|%s|%s] %s\n", id, glexDebugSource(source), glexDebugType(type), glexDebugSeverity(severity), message);
}

static void glexSystemFatalErrorDefault(const char* fmt, va_list args)
{
	vprintf(fmt, args);
	exit(EXIT_FAILURE);
}

static void glexSystemLogPrintDefault(GLEXLogType type, const char* fmt, va_list args)
{
	vprintf(fmt, args);
}

static void* glexSystemAllocMemoryDefault(unsigned int size)
{
	if (size < 1)
		return NULL;

	return malloc(size);
}

static void glexSystemFreeMemoryDefault(void* ptr)
{
	if (ptr != NULL)
		free(ptr);
}

static GLboolean glexInstallConfig(const GLEXConfig* config)
{
	glexConfig.system.LogPrint = glexSystemLogPrintDefault;
	glexConfig.system.FatalError = glexSystemFatalErrorDefault;
	glexConfig.system.AllocMemory = glexSystemAllocMemoryDefault;
	glexConfig.system.FreeMemory = glexSystemFreeMemoryDefault;

	if (config == NULL)
		return GL_TRUE;

	if (config == NULL)
		return 0;

	if (config->system.AllocMemory != NULL && config->system.FreeMemory == NULL)
		return -1;

	if (config->system.AllocMemory == NULL && config->system.FreeMemory != NULL)
		return -1;

	if (config->system.AllocMemory != NULL && config->system.FreeMemory != NULL) {
		glexConfig.system.AllocMemory = config->system.AllocMemory;
		glexConfig.system.FreeMemory = config->system.FreeMemory;
	}

	if (config->system.LogPrint != NULL)
		glexConfig.system.LogPrint = config->system.LogPrint;

	if (config->system.FatalError != NULL)
		glexConfig.system.FatalError = config->system.FatalError;

	return GL_TRUE;
}

void glexFatalError(const char *fmt, ...)
{
	va_list args;

	GLEX_ASSERT(glexConfig.system.FatalError != NULL);

	va_start(args, fmt);
	glexConfig.system.FatalError(fmt, args);
	va_end(args);
}

void glexLogPrint(GLEXLogType type, const char* fmt, va_list args)
{
	GLEX_ASSERT(glexConfig.system.LogPrint != NULL);

	glexConfig.system.LogPrint(type, fmt, args);
}

void *glexAllocMemory(unsigned int size)
{
	GLEX_ASSERT(glexConfig.system.AllocMemory != NULL);

	return glexConfig.system.AllocMemory(size);
}

void glexFreeMemory(void *ptr)
{
	GLEX_ASSERT(glexConfig.system.FreeMemory != NULL);

	glexConfig.system.FreeMemory(ptr);
}

GLEX_API int glexInit(const GLEXConfig *config)
{
	if (!glexInstallConfig(config))
		return -1;

	if (GL_KHR_debug)
		glDebugMessageCallback(glexDebugOutput, NULL);

	if (!glexInitProgram()) {
		glexLogError("Failed to initialize program.\n");
		goto bad0;
	}

	if (!glexInitMaterial()) {
		glexLogError("Failed to initialize material.\n");
		goto bad1;
	}

	if (!glexInitLight()) {
		glexLogError("Failed to initialize light.\n");
		goto bad2;
	}

	if (!glexInitMesh()) {
		glexLogError("Failed to initialize mesh.\n");
		goto bad3;
	}

	if (!glexInitTest()) {
		glexLogError("Failed to initialize test.\n");
		goto bad4;
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return 0;

bad4:
	glexShutdownMesh();

bad3:
	glexShutdownLight();

bad2:
	glexShutdownMaterial();

bad1:
	glexShutdownProgram();

bad0:
	return -1;
}

GLEX_API void glexShutdown(void)
{
	glexShutdownTest();
	glexShutdownMesh();
	glexShutdownLight();
	glexShutdownMaterial();
	glexShutdownProgram();
}

GLEX_API void glexViewport(int x, int y, int width, int height)
{

}

GLEX_API void glexClear(void)
{

}

GLEX_API void glexRender(void)
{
}
