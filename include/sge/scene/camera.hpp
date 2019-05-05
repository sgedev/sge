//
//
#ifndef SGE_SCENE_CAMERA_HPP
#define SGE_SCENE_CAMERA_HPP

#include <sge/db.hpp>
#include <sge/renderer.hpp>
#include <sge/scene/common.hpp>
#include <sge/scene/node.hpp>

SGE_SCENE_BEGIN

class camera: public node {
public:
	camera(void);
	virtual ~camera(void);

public:
	bool create(void);
	void destroy(void);
	void reset(void);
	void mouse_look(float dx, float dy);
	float move_speed(void);
	void set_move_speed(float v);
	void move_forward(void);
	void move_backward(void);
	void move_left(void);
	void move_right(void);
	float fov(void);
	void set_fov(float v);

private:
	renderer::view m_view;
	glm::vec3 m_pos;
	glm::vec3 m_direction;
	glm::vec3 m_up;
	glm::vec2 m_rotate;
	float m_fov;
	float m_move_speed;
};

SGE_SCENE_END

#endif // SGE_SCENE_CAMERA_HPP

