/* 
 *
 */
#include "glex_common.h"

GLboolean glexInitTest(GLEXContext *context)
{
	GLEX_ASSERT(context != NULL);

	GLfloat test_vertex[] = {
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f
	};

	GLuint test_index[] = {
		0, 3, 2,
		0, 1, 3,
		1, 5, 3,
		3, 5, 7,
		0, 2, 4,
		4, 2, 6,
		2, 3, 6,
		3, 7, 6,
		5, 4, 6,
		7, 5, 4,
		0, 4, 1,
		1, 4, 5
	};

	glGenBuffers(1, &context->test_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, context->test_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(test_vertex), test_vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &context->test_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context->test_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(test_index), test_index, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &context->test_vertex_array);
	glBindVertexArray(context->test_vertex_array);

	glBindBuffer(GL_ARRAY_BUFFER, context->test_vertex_array);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context->test_index_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	return GL_TRUE;
}

void glexShutdownTest(GLEXContext *context)
{
	GLEX_ASSERT(context != NULL);

	glDeleteBuffers(1, &context->test_vertex_array);
	glDeleteBuffers(1, &context->test_index_buffer);
	glDeleteBuffers(1, &context->test_vertex_buffer);
}

void glexDrawTest(void)
{
	float x, y, z;
	float size = 50.0f;
	float step = 4.0f;
	hmm_mat4 m;

	GLEX_ASSERT(glex != NULL);

	glBindVertexArray(glex->test_vertex_array);

	for (x = 0.0f; x < size; x += step) {
		for (y = 0.0f; y < size; y += step) {
			for (z = 0.0f; z < size; z += step) {
				hmm_vec3 pos = { x, y, z };
				m = HMM_Translate(pos);
				glUniformMatrix4fv(glex->uniform_mat4_loc[GLEX_UNIFORM_MODEL_MATRIX], 1, GL_FALSE, (const GLfloat *)(m.Elements));
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}
		}
	}

	glBindVertexArray(0);
}

