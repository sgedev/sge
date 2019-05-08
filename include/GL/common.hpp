//
//
#ifndef GL_COMMON_HPP
#define GL_COMMON_HPP

#include <GL/glew.h>

#include <glm.hpp>

#ifndef GL_ASSERT
#include <assert.h>
#define GL_ASSERT(expr) assert(expr)
#endif

#define GL_BEGIN namespace GL {
#define GL_END }

#endif // SGE_GL_COMMON_HPP

