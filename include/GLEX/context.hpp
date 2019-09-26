//
//
#ifndef GLEX_CONTEXT_HPP
#define GLEX_CONTEXT_HPP

#include <GLEX/common.hpp>

GLEX_BEGIN

class Context {
public:
	typedef enum {
		UNIFORM_ELAPSED = 0,
		UNIFORM_FLOAT_MAX
	} UniformFloat;

	typedef enum {
		UNIFORM_MODEL_MATRIX = 0,
		UNIFORM_VIEW_MATRIX,
		UNIFORM_PROJECTION_MATRIX,
		UNIFORM_MAT4_MAX
	} UniformMat4;

public:
	Context(void);
	virtual ~Context(void);

public:
	bool Init(void);
	void Shutdown(void);
	void BeginFrame(void);
	void EndFrame(void);

private:
	bool InitProgram(void);
	void ShutdownProgram(void);
	void CommitUniforms(void);

private:
	bool InitTest(void);
	void ShutdownTest(void);
	void DrawTest(void);

private:
	GLuint m_vertex_shader;
	GLuint m_fragment_shader;
	GLuint m_program;
	GLint m_uniform_float_loc[UNIFORM_FLOAT_MAX];
	GLfloat m_uniform_float[UNIFORM_FLOAT_MAX];
	GLint m_uniform_mat4_loc[UNIFORM_MAT4_MAX];
	glm::mat4 m_uniform_mat4[UNIFORM_MAT4_MAX];

	GLuint m_test_vertex_buffer;
	GLuint m_test_index_buffer;
	GLuint m_test_vertex_array;
};

GLEX_END

#endif // GLEX_CONTEXT_HPP

