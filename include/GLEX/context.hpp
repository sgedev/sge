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
	bool init(void);
	void shutdown(void);
	void beginFrame(void);
	void endFrame(void);

private:
	bool initProgram(void);
	void shutdownProgram(void);
	void commitUniforms(void);

private:
	bool initTest(void);
	void shutdownTest(void);
	void drawTest(void);

private:
	GLuint m_vertexShader;
	GLuint m_fragmentShader;
	GLuint m_program;
	GLint m_uniformFloatLoc[UNIFORM_FLOAT_MAX];
	GLfloat m_uniformFloat[UNIFORM_FLOAT_MAX];
	GLint m_uniformMat4Loc[UNIFORM_MAT4_MAX];
	glm::mat4 m_uniformMat4[UNIFORM_MAT4_MAX];

	GLuint m_testVertexBuffer;
	GLuint m_testIndexBuffer;
	GLuint m_testVertexArray;
};

GLEX_END

#endif // GLEX_CONTEXT_HPP

