/* 
 *
 */
#include <string.h>

#include "glex_program.h"

typedef enum {
	GLEX_UNIFORM_ELAPSED = 0,
	GLEX_UNIFORM_FLOAT_MAX
} GELXUniformFloat;

typedef enum {
	GLEX_UNIFORM_MODEL_MATRIX = 0,
	GLEX_UNIFORM_VIEW_MATRIX,
	GLEX_UNIFORM_PROJECTION_MATRIX,
	GLEX_UNIFORM_MAT4_MAX
} GELXUniformMat4;

static GLuint glexVertexShader;
static GLuint glexFragmentShader;
static GLuint glexProgram;
static GLint glexUniformFloatLoc[GLEX_UNIFORM_FLOAT_MAX];
static GLfloat glexUniformFloat[GLEX_UNIFORM_FLOAT_MAX];
static GLint glexUniformMat4Loc[GLEX_UNIFORM_MAT4_MAX];
static hmm_mat4 glexUniformMat4[GLEX_UNIFORM_MAT4_MAX];

const char *glexVertexShaderSource =
	"#version 330\n"
	"uniform mat4 glex_ModelMatrix;\n"
	"uniform mat4 glex_ViewMatrix;\n"
	"uniform mat4 glex_ProjectionMatrix;\n"
	"layout (location = 0) in vec4 Position;\n"
	"void main() {\n"
	"	gl_Position = glex_ProjectionMatrix * glex_ViewMatrix * glex_ModelMatrix * Position;\n"
	"}\n"
	;

const char *glexFragmentShaderSource =
	"#version 330\n"
	"void main() {\n"
	"	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
	"}\n"
	;

static GLuint glexCreateShader(GLenum type, const char *src)
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

static GLuint glexCreateProgram(GLuint vshader, GLuint fshader)
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

GLboolean glexInitProgram(void)
{
	glexVertexShader = glexCreateShader(GL_VERTEX_SHADER, glexVertexShaderSource);
	if (glexVertexShader == 0)
		goto bad0;

	glexFragmentShader = glexCreateShader(GL_FRAGMENT_SHADER, glexFragmentShaderSource);
	if (glexFragmentShader == 0)
		goto bad1;

	glexProgram = glexCreateProgram(glexVertexShader, glexFragmentShader);
	if (glexProgram == 0)
		goto bad2;

	glexUniformFloatLoc[GLEX_UNIFORM_ELAPSED] = glGetUniformLocation(glexProgram, "glex_Elapsed");
	glexUniformMat4Loc[GLEX_UNIFORM_MODEL_MATRIX] = glGetUniformLocation(glexProgram, "glex_ModelMatrix");
	glexUniformMat4Loc[GLEX_UNIFORM_VIEW_MATRIX] = glGetUniformLocation(glexProgram, "glex_ViewMatrix");
	glexUniformMat4Loc[GLEX_UNIFORM_PROJECTION_MATRIX] = glGetUniformLocation(glexProgram, "glex_ProjectionMatrix");

	glexUniformMat4[GLEX_UNIFORM_MODEL_MATRIX] = HMM_Mat4d(1.0f);
	glexUniformMat4[GLEX_UNIFORM_VIEW_MATRIX] = HMM_Mat4d(1.0f);
	glexUniformMat4[GLEX_UNIFORM_PROJECTION_MATRIX] = HMM_Mat4d(1.0f);

	return GL_TRUE;

bad2:
	glDeleteShader(glexFragmentShader);
	glexFragmentShader = 0;

bad1:
	glDeleteShader(glexVertexShader);
	glexVertexShader = 0;

bad0:
	return GL_FALSE;
}

void glexShutdownProgram(void)
{
	glDeleteShader(glexFragmentShader);
	glexFragmentShader = 0;

	glDeleteShader(glexVertexShader);
	glexVertexShader = 0;
}

void glexCommitUniforms(void)
{
	for (int i = 0; i < GLEX_UNIFORM_FLOAT_MAX; ++i) {
		if (glexUniformFloatLoc[i] >= 0)
			glUniform1f(glexUniformFloatLoc[i], glexUniformFloat[i]);
	}

	for (int i = 0; i < GLEX_UNIFORM_MAT4_MAX; ++i) {
		if (glexUniformMat4Loc[i] >= 0)
			glUniformMatrix4fv(glexUniformMat4Loc[i], 1, GL_FALSE,
				(const GLfloat *)(glexUniformMat4[i].Elements));
	}
}

GLEX_API void glexModelMatrix(float *m)
{
	GLEX_ASSERT(m != NULL);

	memcpy(glexUniformMat4[GLEX_UNIFORM_MODEL_MATRIX].Elements, m, sizeof(float) * 16);
}

GLEX_API void glexViewMatrix(float *m)
{
	GLEX_ASSERT(m != NULL);

	memcpy(glexUniformMat4[GLEX_UNIFORM_VIEW_MATRIX].Elements, m, sizeof(float) * 16);
}

GLEX_API void glexProjectionMatrix(float *m)
{
	GLEX_ASSERT(m != NULL);

	memcpy(glexUniformMat4[GLEX_UNIFORM_PROJECTION_MATRIX].Elements, m, sizeof(float) * 16);
}

