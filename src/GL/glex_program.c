/* 
 *
 */
#include <string.h>

#include "glex_common.h"

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

GLboolean glexInitProgram(GLEXContext *context)
{
	GLEX_ASSERT(context != NULL);

	context->vertex_shader = glexCreateShader(GL_VERTEX_SHADER, glexVertexShaderSource);
	if (context->vertex_shader == 0)
		goto bad0;

	context->fragment_shader = glexCreateShader(GL_FRAGMENT_SHADER, glexFragmentShaderSource);
	if (context->fragment_shader == 0)
		goto bad1;

	context->program = glexCreateProgram(context->vertex_shader, context->fragment_shader);
	if (context->program == 0)
		goto bad2;

	context->uniform_float_loc[GLEX_UNIFORM_ELAPSED] = glGetUniformLocation(context->program, "glex_Elapsed");
	context->uniform_float[GLEX_UNIFORM_ELAPSED] = 1.0f;

	context->uniform_mat4_loc[GLEX_UNIFORM_MODEL_MATRIX] = glGetUniformLocation(context->program, "glex_ModelMatrix");
	context->uniform_mat4[GLEX_UNIFORM_MODEL_MATRIX] = HMM_Mat4d(1.0f);

	context->uniform_mat4_loc[GLEX_UNIFORM_VIEW_MATRIX] = glGetUniformLocation(context->program, "glex_ViewMatrix");
	context->uniform_mat4[GLEX_UNIFORM_VIEW_MATRIX] = HMM_Mat4d(1.0f);

	context->uniform_mat4_loc[GLEX_UNIFORM_PROJECTION_MATRIX] = glGetUniformLocation(context->program, "glex_ProjectionMatrix");
	context->uniform_mat4[GLEX_UNIFORM_PROJECTION_MATRIX] = HMM_Perspective(90.0f, 4.0f / 3.0f, 0.1f, 80.0f);

	return GL_TRUE;

bad2:
	glDeleteShader(context->fragment_shader);
	context->fragment_shader = 0;

bad1:
	glDeleteShader(context->vertex_shader);
	context->vertex_shader = 0;

bad0:
	return GL_FALSE;
}

void glexShutdownProgram(GLEXContext *context)
{
	GLEX_ASSERT(context != NULL);

	glDeleteShader(context->fragment_shader);
	context->fragment_shader = 0;

	glDeleteShader(context->vertex_shader);
	context->vertex_shader = 0;
}

void glexCommitUniforms(void)
{
	GLEX_ASSERT(glex != NULL);

	for (int i = 0; i < GLEX_UNIFORM_FLOAT_MAX; ++i) {
		if (glex->uniform_float_loc[i] >= 0)
			glUniform1f(glex->uniform_float_loc[i], glex->uniform_float[i]);
	}

	for (int i = 0; i < GLEX_UNIFORM_MAT4_MAX; ++i) {
		if (glex->uniform_mat4_loc[i] >= 0)
			glUniformMatrix4fv(glex->uniform_mat4_loc[i], 1, GL_FALSE,
				(const GLfloat *)(glex->uniform_mat4[i].Elements));
	}
}

GLEX_API void glexModelMatrix(float *m)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(m != NULL);

	memcpy(glex->uniform_mat4[GLEX_UNIFORM_MODEL_MATRIX].Elements, m, sizeof(float) * 16);
}

GLEX_API void glexViewMatrix(float *m)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(m != NULL);

	memcpy(glex->uniform_mat4[GLEX_UNIFORM_VIEW_MATRIX].Elements, m, sizeof(float) * 16);
}

GLEX_API void glexProjectionMatrix(float *m)
{
	GLEX_ASSERT(glex != NULL);
	GLEX_ASSERT(m != NULL);

	memcpy(glex->uniform_mat4[GLEX_UNIFORM_PROJECTION_MATRIX].Elements, m, sizeof(float) * 16);
}
