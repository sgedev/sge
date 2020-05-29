//
//
#ifndef SGE_GRAPHICS_COMMON_HPP
#define SGE_GRAPHICS_COMMON_HPP

#include <GL/gl3w.h>

#include <SGE/Common.hpp>

#define SGE_GRAPHICS_BEGIN SGE_BEGIN namespace Graphics {
#define SGE_GRAPHICS_END } SGE_END

SGE_GRAPHICS_BEGIN

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

typedef Uint16 VertexIndex;

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_COMMON_HPP
