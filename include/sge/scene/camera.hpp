//
//
#ifndef SGE_SCENE_CAMERA_HPP
#define SGE_SCENE_CAMERA_HPP

#include <sge/scene/camera/common.hpp>

SGE_SCENE_CAMERA_BEGIN

bool init(void);
void shutdown(void);
void ortho(float left, float right, float bottom, float top, float znear, float zfar);
void frustum(float left, float right, float bottom, float top, float znear, float nfar);
void look_at(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up);
void look_to(const glm::vec3 &eye, const glm::vec3 &direction, const glm::vec3 &up);
void reset(void);
void render(void);

void draw_test_cube(const glm::mat4 &transform);

SGE_SCENE_CAMERA_END

#endif // SGE_SCENE_CAMERA_HPP

