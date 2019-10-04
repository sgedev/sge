//
//
#include <string.h>

#include <GLEX/context.hpp>

GLEX_BEGIN

static const char *VertexShaderSource =
	"#version 330\n"
	"uniform mat4 glex_ModelMatrix;\n"
	"uniform mat4 glex_ViewMatrix;\n"
	"uniform mat4 glex_ProjectionMatrix;\n"
	"layout (location = 0) in vec4 Position;\n"
	"void main() {\n"
	"	gl_Position = glex_ProjectionMatrix * glex_ViewMatrix * glex_ModelMatrix * Position;\n"
	"}\n"
	;

static const char *FragmentShaderSource =
	"#version 330\n"
	"void main() {\n"
	"	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
	"}\n"
	;

static GLuint createShader(GLenum type, const char *src)
{
	GLuint shader;
	int len[1];
	GLint status;

	GLEX_ASSERT(src != NULL);

	shader = glCreateShader(type);
	if (shader == 0)
		return 0;

	len[0] = (int)strlen(src);
	GLEX_ASSERT(len[0] > 0);

	glShaderSource(shader, 1, &src, len);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (!status) {
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

static GLuint createProgram(GLuint vshader, GLuint fshader)
{
	GLuint program;
	GLint status;

	GLEX_ASSERT(vshader > 0);
	GLEX_ASSERT(fshader > 0);

	program = glCreateProgram();
	if (program == 0)
		return 0;

	glAttachShader(program, vshader);
	glAttachShader(program, fshader);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status) {
		glDeleteProgram(program);
		return 0;
	}

	return program;
}

bool Context::initProgram(void)
{
	m_vertexShader = createShader(GL_VERTEX_SHADER, VertexShaderSource);
	if (m_vertexShader == 0)
		goto bad0;

	m_fragmentShader = createShader(GL_FRAGMENT_SHADER, FragmentShaderSource);
	if (m_fragmentShader == 0)
		goto bad1;

	m_program = createProgram(m_vertexShader, m_fragmentShader);
	if (m_program == 0)
		goto bad2;

	m_uniformFloatLoc[UNIFORM_ELAPSED] = glGetUniformLocation(m_program, "glex_Elapsed");
	m_uniformFloat[UNIFORM_ELAPSED] = 1.0f;

	m_uniformMat4Loc[UNIFORM_MODEL_MATRIX] = glGetUniformLocation(m_program, "glex_ModelMatrix");
	m_uniformMat4[UNIFORM_MODEL_MATRIX] = glm::mat4(1.0f);

	m_uniformMat4Loc[UNIFORM_VIEW_MATRIX] = glGetUniformLocation(m_program, "glex_ViewMatrix");
	m_uniformMat4[UNIFORM_VIEW_MATRIX] = glm::mat4(1.0f);

	m_uniformMat4Loc[UNIFORM_PROJECTION_MATRIX] = glGetUniformLocation(m_program, "glex_ProjectionMatrix");
	m_uniformMat4[UNIFORM_PROJECTION_MATRIX] = glm::perspective(90.0f, 4.0f / 3.0f, 0.1f, 80.0f);

	return true;

bad2:
	glDeleteShader(m_fragmentShader);
	m_fragmentShader = 0;

bad1:
	glDeleteShader(m_vertexShader);
	m_vertexShader = 0;

bad0:
	return false;
}

void Context::shutdownProgram(void)
{
	glDeleteShader(m_fragmentShader);
	m_fragmentShader = 0;

	glDeleteShader(m_vertexShader);
	m_vertexShader = 0;
}

void Context::commitUniforms(void)
{
	for (int i = 0; i < UNIFORM_FLOAT_MAX; ++i) {
		if (m_uniformFloatLoc[i] >= 0)
			glUniform1f(m_uniformFloatLoc[i], m_uniformFloat[i]);
	}

	for (int i = 0; i < UNIFORM_MAT4_MAX; ++i) {
		if (m_uniformMat4Loc[i] >= 0)
			glUniformMatrix4fv(m_uniformMat4Loc[i], 1, GL_FALSE, glm::value_ptr(m_uniformMat4[i]));
	}
}

GLEX_END

