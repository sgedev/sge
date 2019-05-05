//
//
#ifndef SGE_RENDERER_VIEW_HPP
#define SGE_RENDERER_VIEW_HPP

#include <imgui.h>

#include <sge/noncopyable.hpp>
#include <sge/gl/context.hpp>
#include <sge/renderer/common.hpp>

SGE_RENDERER_BEGIN

class view: public noncopyable {
public:
	view(void);
	virtual ~view(void);

public:
	bool create(gl::context &gl_context);
	void destroy(void);
	void reset(void);
	void render(void);
	void set_viewport(const glm::ivec4 &v);
	void set_view_matrix(const glm::mat4 &v);
	void set_projection_matrix(const glm::mat4 &v);

private:
	bool init_program(void);
	void commit_uniforms(void);
	void init_test(void);
	void shutdown_test(void);
	void draw_test(void);

private:
	gl::context *m_gl_context;
	SDL_Window *m_window;
	ImGuiContext *m_imgui_context;
	glm::ivec4 m_viewport;
	bool m_clear_enabled;
	glm::vec3 m_clear_color;
	gl::program m_program;

	gl::buffer m_test_vertex;
	gl::buffer m_test_index;
	gl::vertex_array m_test_vertex_array;

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

	int m_uniform_float_loc[UNIFORM_FLOAT_MAX];
	float m_uniform_float[UNIFORM_FLOAT_MAX];

	int m_uniform_mat4_loc[UNIFORM_MAT4_MAX];
	glm::mat4 m_uniform_mat4[UNIFORM_MAT4_MAX];
};

inline void view::set_viewport(const glm::ivec4 &v)
{
	m_viewport = v;
}

inline void view::set_view_matrix(const glm::mat4 &v)
{
	m_uniform_mat4[UNIFORM_VIEW_MATRIX] = v;
}

inline void view::set_projection_matrix(const glm::mat4 &v)
{
	m_uniform_mat4[UNIFORM_PROJECTION_MATRIX] = v;
}

SGE_RENDERER_END

#endif // SGE_RENDERER_VIEW_HPP
