/*
 *
 */
#ifndef GLEX_H
#define GLEX_H

#include <stdarg.h>

#include <GL/gl3w.h>
#include <GL/glexconfig.h>

#if defined(_MSC_VER)
#	define GLEX_DLLEXPORT __declspec(dllexport)
#	define GLEX_DLLIMPORT __declspec(dllimport)
#elif defined(__GNUC__)
#	define GLEX_DLLEXPORT
#	define GLEX_DLLIMPORT
#else
#	error unknown compiler.
#endif

#ifdef GLEX_SHARED
#	ifdef GLEX
#		define GLEX_API GLEX_DLLEXPORT
#	else
#		define GLEX_API GLEX_DLLIMPORT
#	endif
#else
#	define GLEX_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum {
	GLEX_OK = 0,
	GLEX_INVALID_PARAM,
	GLEX_UNKNOWN
};

typedef enum {
	GLEX_LOG_NONE = 0,
	GLEX_LOG_ERROR,
	GLEX_LOG_WARNING,
	GLEX_LOG_INFO,
	GLEX_LOG_DEBUG
} GLEXLogLevel;

typedef struct {
	void (*logWrite)(GLEXLogLevel level, const char *fmt, va_list args);
	void *(*allocMemory)(unsigned int size);
	void (*freeMemory)(void *ptr);
} GLEXSystem;

typedef struct GLEXContext_ GLEXContext;

GLEX_API int glexInit(const GLEXSystem *system);
GLEX_API void glexShutdown(void);
GLEX_API void glexLogLevel(GLEXLogLevel level);
GLEX_API GLEXLogLevel glexGetLogLevel(void);

GLEX_API GLEXContext *glexCreateContext(GL3WGetProcAddressProc proc);
GLEX_API void glexDeleteContext(GLEXContext *context);
GLEX_API GLEXContext *glexGetCurrentContext(void);
GLEX_API void glexMakeCurrent(GLEXContext *context);

GLEX_API void glexBeginFrame(void);
GLEX_API void glexEndFrame(void);

GLEX_API void glexViewport(int x, int y, int width, int height);
GLEX_API void glexElapsed(GLfloat v);
GLEX_API void glexModelMatrix(const GLfloat *v);
GLEX_API void glexViewMatrix(const GLfloat *v);
GLEX_API void glexProjectionMatrix(const GLfloat *v);

GLEX_API void glexBeginPath(void);
GLEX_API void glexClosePath(void);

GLEX_API void glexColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLEX_API void glexFillColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLEX_API void glexLineWidth(GLfloat width);

GLEX_API void glexFill(void);
GLEX_API void glexMoveTo(GLfloat x, GLfloat y, GLfloat z);
GLEX_API void glexLineTo(GLfloat x, GLfloat y, GLfloat z);
GLEX_API void glexRect(GLfloat x, GLfloat y, GLfloat width, GLfloat height);

GLEX_API GLuint glexCreateFont(void);
GLEX_API void glexDeleteFont(GLuint id);
GLEX_API GLboolean glexIsFont(GLuint id);
GLEX_API GLboolean glexLoadFontFromFile(GLuint id, const char *filename);
GLEX_API GLboolean glexLoadFontFromMem(GLuint id, const void *ptr, GLsizei len);

GLEX_API void glexFont(GLuint id);
GLEX_API void glexText(GLfloat x, GLfloat y, const char *text);

GLEX_API GLuint glexCreateMesh(void);
GLEX_API void glexDeleteMesh(GLuint id);
GLEX_API GLboolean glexIsMesh(GLuint id);

GLEX_API GLuint glexCreateMaterial(void);
GLEX_API void glexDeleteMaterial(GLuint id);
GLEX_API GLboolean glexIsMaterial(GLuint id);

#ifdef __cplusplus
}
#endif

#endif /* GLEX_H */

