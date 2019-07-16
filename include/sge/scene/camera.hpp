//
//
#ifndef SGE_SCENE_CAMERA_HPP
#define SGE_SCENE_CAMERA_HPP

#include <sge/gl.hpp>
#include <sge/scene/common.hpp>
#include <sge/scene/node.hpp>

SGE_SCENE_BEGIN

class camera: public node {
public:
	camera(node *parent = NULL);
	virtual ~camera(void);

public:
	bool create(void);
	void destroy(void);
	const glm::ivec4 &viewport(void) const;
	void set_viewport(int x, int y, int width, int height);
	void set_viewport(const glm::ivec4 &v);
	const glm::mat4 &projection_matrix(void) const;
	void set_projection_matrix(const glm::mat4 &v);
	void render(void);

private:
	void commit_uniforms(void);
	static bool init_program(void);
	static void init_test(void);
	static void shutdown_test(void);
	static void draw_test(void);

private:
	glm::ivec4 m_viewport;
	glm::mat4 m_projection_matrix;
	static gl::program c_program;
	static gl::buffer c_test_vertex;
	static gl::buffer c_test_index;
	static gl::vertex_array c_test_vertex_array;
	static int c_ref;
};

inline const glm::ivec4 &camera::viewport(void) const
{
	return m_viewport;
}

inline void camera::set_viewport(int x, int y, int width, int height)
{
	set_viewport(glm::ivec4(x, y, width, height));
}

inline void camera::set_viewport(const glm::ivec4 &v)
{
	m_viewport = v;
}

inline const glm::mat4 &camera::projection_matrix(void) const
{
	return m_projection_matrix;
}

inline void camera::set_projection_matrix(const glm::mat4 &v)
{
	m_projection_matrix = v;
}

SGE_SCENE_END

#endif // SGE_SCENE_CAMERA_HPP

