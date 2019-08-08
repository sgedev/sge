//
//
#ifndef SGE_RENDERER_HPP
#define SGE_RENDERER_HPP

#include <GL/gl.hpp>

#include <sge/common.hpp>
#include <sge/window.hpp>
#include <sge/view.hpp>

SGE_BEGIN


class renderer {
public:
	renderer(void);
	virtual ~renderer(void);

public:
	bool init(window *pw);
	void shutdown(void);
	void draw(view &v);

private:
	bool setup_program(void);
	void commit_uniforms(void);
	void init_test(void);
	void shutdown_test(void);
	void draw_test(void);

private:
	enum uniform_float {
		UNIFORM_ELAPSED = 0,
		UNIFORM_FLOAT_MAX
	};

	enum uniform_mat4 {
		UNIFORM_MODEL_MATRIX = 0,
		UNIFORM_VIEW_MATRIX,
		UNIFORM_PROJECTION_MATRIX,
		UNIFORM_NORMAL_MATRIX,
		UNIFORM_MAT4_MAX
	};

	GL::Program m_program;

	// float uniforms
	int m_uniform_float_loc[UNIFORM_FLOAT_MAX];
	float m_uniform_float[UNIFORM_FLOAT_MAX];

	// mat4 uniforms
	int m_uniform_mat4_loc[UNIFORM_MAT4_MAX];
	glm::mat4 m_uniform_mat4[UNIFORM_MAT4_MAX];

	GL::Buffer m_test_vertex;
	GL::Buffer m_test_index;
	GL::VertexArray m_test_vertex_array;
};


SGE_END

#endif // SGE_RENDERER_HPP

