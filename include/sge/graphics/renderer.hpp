//
//
#ifndef SGE_GRAPHICS_RENDERER_HPP
#define SGE_GRAPHICS_RENDERER_HPP

#include <sge/graphics/common.hpp>
#include <sge/graphics/canvas.hpp>
#include <sge/graphics/light.hpp>
#include <sge/graphics/mesh.hpp>
#include <sge/graphics/material.hpp>

SGE_GRAPHICS_BEGIN

class renderer {
public:
	renderer(canvas *target);
	virtual ~renderer(void);

public:
	bool init(void);
	void shutdown(void);
	void begin(void);
	void end(void);

public:
    enum matrix_type {
        MATRIX_MODEL = 0,
        MATRIX_VIEW,
        MATRIX_PROJECTION
    };

    void matrix_mode(matrix_type mt);
    void push_matrix(void);
    void pop_matrix(void);
    void load_identity(void);
    void translate(const glm::vec3 &r);
    void translate(float x, float y, float z);
    void scale(const glm::vec3 &r);
    void scale(float x, float y, float z);
    void rotate(const glm::quat &r);
    void rotate(float angle, float x, float y, float z);
    void look_at(const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &up);
    void look_at(float posx, float posy, float posz, float dirx, float diry, float dirz, float upx, float upy, float upz);
    void ortho(float left, float right, float bottom, float top, float znear, float zfar);
    void frustum(float left, float right, float bottom, float top, float znear, float zfar);
    void perpective(float fov, float ratio, float znear, float zfar);
    light &ambient_light(void);
    // void set_material()
    // void add(mesh *p);
    // void add(direction_light *p);
    // void add(point_light *p);
    // void add(spot_light *p);

private:
    matrix_type m_matrix_mode;
    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::quat m_rotation;
    glm::vec3 m_view_position;
    glm::vec3 m_view_direction;
    glm::vec3 m_view_up;
    glm::ivec4 m_view_rect;
    float m_view_fov;
    light m_ambient_light;
    canvas *m_target;
};

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_RENDERER_HPP
