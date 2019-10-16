/*
 *
 */
#include <string.h>

#include "glex_context.h"

static const char *glexVertexShaderSource =
	"#version 330\n"
	"uniform mat4 glex_ModelMatrix;\n"
	"uniform mat4 glex_ViewMatrix;\n"
	"uniform mat4 glex_ProjectionMatrix;\n"
	"layout (location = 0) in vec4 Position;\n"
	"void main() {\n"
	"	gl_Position = glex_ProjectionMatrix * glex_ViewMatrix * glex_ModelMatrix * Position;\n"
	"}\n"
	;

static const char *glexFragmentShaderSource =
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

	GLEX_ASSERT(context->vertexShader == 0);
	context->vertexShader = glexCreateShader(GL_VERTEX_SHADER, glexVertexShaderSource);
	if (context->vertexShader == 0)
		goto bad0;

	GLEX_ASSERT(context->fragmentShader == 0);
	context->fragmentShader = glexCreateShader(GL_FRAGMENT_SHADER, glexFragmentShaderSource);
	if (context->fragmentShader == 0)
		goto bad1;

	GLEX_ASSERT(context->program == 0);
	context->program = glexCreateProgram(context->vertexShader, context->fragmentShader);
	if (context->program == 0)
		goto bad2;

	context->uniformFloatLoc[GLEX_UNIFORM_ELAPSED] = glGetUniformLocation(context->program, "glex_Elapsed");
	context->uniformFloat[GLEX_UNIFORM_ELAPSED] = 1.0f;

	context->uniformMat4Loc[GLEX_UNIFORM_MODEL_MATRIX] = glGetUniformLocation(context->program, "glex_ModelMatrix");
	context->uniformMat4[GLEX_UNIFORM_MODEL_MATRIX] = HMM_Mat4d(1.0f);

	context->uniformMat4Loc[GLEX_UNIFORM_VIEW_MATRIX] = glGetUniformLocation(context->program, "glex_ViewMatrix");
	context->uniformMat4[GLEX_UNIFORM_VIEW_MATRIX] = HMM_Mat4d(1.0f);

	context->uniformMat4Loc[GLEX_UNIFORM_PROJECTION_MATRIX] = glGetUniformLocation(context->program, "glex_ProjectionMatrix");
	context->uniformMat4[GLEX_UNIFORM_PROJECTION_MATRIX] = HMM_Perspective(90.0f, 4.0f / 3.0f, 0.1f, 80.0f);

	return GL_TRUE;

bad2:
	glDeleteShader(context->fragmentShader);
	context->fragmentShader = 0;

bad1:
	glDeleteShader(context->vertexShader);
	context->vertexShader = 0;

bad0:
	return GL_FALSE;
}

void glexShutdownProgram(GLEXContext *context)
{
	GLEX_ASSERT(context != NULL);

	GLEX_ASSERT(context->program != 0);
	glDeleteProgram(context->program);
	context->program = 0;

	GLEX_ASSERT(context->fragmentShader != 0);
	glDeleteShader(context->fragmentShader);
	context->fragmentShader = 0;

	GLEX_ASSERT(context->vertexShader != 0);
	glDeleteShader(context->vertexShader);
	context->vertexShader = 0;
}

void glexCommitUniforms(void)
{
	GLEX_ASSERT(glex != NULL);

	for (int i = 0; i < GLEX_UNIFORM_FLOAT_MAX; ++i) {
		if (glex->uniformFloatLoc[i] >= 0)
			glUniform1f(glex->uniformFloatLoc[i], glex->uniformFloat[i]);
	}

	for (int i = 0; i < GLEX_UNIFORM_MAT4_MAX; ++i) {
		if (glex->uniformMat4Loc[i] >= 0)
			glUniformMatrix4fv(glex->uniformMat4Loc[i], 1, GL_FALSE,
				(const GLfloat *)(glex->uniformMat4[i].Elements));
	}
}

