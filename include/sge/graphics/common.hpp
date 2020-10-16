//
//
#ifndef SGE_GRAPHICS_COMMON_HPP
#define SGE_GRAPHICS_COMMON_HPP

#include <GL/gl3w.h>
#include <nanovg.h>

#include <sge/common.hpp>

#define SGE_GRAPHICS_BEGIN SGE_BEGIN namespace graphics {
#define SGE_GRAPHICS_END } SGE_END

SGE_GRAPHICS_BEGIN

typedef struct {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tex_coord;
} vertex_t;

typedef uint32_t vertex_index_t;

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_COMMON_HPP
