//
//
#include <sge/scene/camera.hpp>

SGE_SCENE_BEGIN

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

// float uniforms
static int c_uniform_float_loc[UNIFORM_FLOAT_MAX];
static float c_uniform_float[UNIFORM_FLOAT_MAX];

// mat4 uniforms
static int c_uniform_mat4_loc[UNIFORM_MAT4_MAX];
static glm::mat4 c_uniform_mat4[UNIFORM_MAT4_MAX];

static const char *c_vertex_shader_source =
	"#version 330\n"
	"uniform mat4 sge_model_matrix;\n"
	"uniform mat4 sge_view_matrix;\n"
	"uniform mat4 sge_projection_matrix;\n"
	"layout (location = 0) in vec4 position;\n"
	"void main() {\n"
	"	gl_Position = sge_projection_matrix * sge_view_matrix * sge_model_matrix * position;\n"
	"}\n"
	;

static const char *c_fragment_shader_source =
	"#version 330\n"
	"void main() {\n"
	"	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
	"}\n"
	;

gl::program camera::c_program;
gl::buffer camera::c_test_vertex(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
gl::buffer camera::c_test_index(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
gl::vertex_array camera::c_test_vertex_array;
int camera::c_ref = 0;

camera::camera(node *parent)
	: node(parent)
{
}

camera::~camera(void)
{
}

bool camera::create(void)
{
	SGE_ASSERT(c_ref >= 0);

	if (c_ref > 0) {
		c_ref++;
		return true;
	}

	if (!init_program()) {
		SGE_LOGE("Failed to initialize ImGui.\n");
		return false;
	}

	c_uniform_mat4[UNIFORM_MODEL_MATRIX] = glm::mat4(1.0f);
	c_uniform_mat4[UNIFORM_VIEW_MATRIX] = glm::mat4(1.0f);
	c_uniform_mat4[UNIFORM_PROJECTION_MATRIX] = glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 0.5f, 999.0f);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	init_test();

	c_ref++;

	return true;
}

void camera::shutdown(void)
{
	SGE_ASSERT(c_ref > 0);

	if (--c_ref > 0)
		return;

	shutdown_test();
	s_program.destroy();
}

void camera::render(void)
{
	draw_test();
}

bool camera::init_program(void)
{
	if (!c_program.create())
		return false;

	SGE_LOGD("Building vertex shader...\n");
	if (!c_program.add_shader(GL_VERTEX_SHADER, c_vertex_shader_source)) {
		SGE_LOGE("Failed to build vertex shader: %s\n", c_program.info_log());
		c_program.destroy();
		return false;
	}

	SGE_LOGD("Building fragment shader...\n");
	if (!c_program.add_shader(GL_FRAGMENT_SHADER, c_fragment_shader_source)) {
		SGE_LOGE("Failed to build fragment shader: %s\n", c_program.info_log());
		c_program.destroy();
		return false;
	}

	SGE_LOGD("Linking program...\n");
	if (!c_program.link()) {
		SGE_LOGE("Failed to link program: %s\n", c_program.info_log());
		c_program.destroy();
		return false;
	}

	// clearing uniform locations...

	for (int i = 0; i < UNIFORM_FLOAT_MAX; ++i)
		c_uniform_float_loc[i] = -1;

	for (int i = 0; i < UNIFORM_MAT4_MAX; ++i)
		c_uniform_mat4_loc[i] = -1;

	// getting uniform locations...
	c_uniform_mat4_loc[UNIFORM_MODEL_MATRIX] = c_program.uniform_location("sge_model_matrix");
	c_uniform_mat4_loc[UNIFORM_VIEW_MATRIX] = c_program.uniform_location("sge_view_matrix");
	c_uniform_mat4_loc[UNIFORM_PROJECTION_MATRIX] = c_program.uniform_location("sge_projection_matrix");

	return true;
}

void camera::commit_uniforms(void)
{
	// float
	for (int i = 0; i < UNIFORM_FLOAT_MAX; ++i) {
		if (c_uniform_float_loc[i] >= 0)
			c_program.uniform(c_uniform_float_loc[i], c_uniform_float[i]);
	}

	// mat4
	for (int i = 0; i < UNIFORM_MAT4_MAX; ++i) {
		if (c_uniform_mat4_loc[i] >= 0)
			c_program.uniform(c_uniform_mat4_loc[i], 1, false, &c_uniform_mat4[i]);
	}
}

void camera::init_test(void)
{
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

	c_test_vertex.create(test_vertex, sizeof(test_vertex));
	c_test_index.create(test_index, sizeof(test_index));
	c_test_vertex_array.create();

	c_test_vertex_array.bind();
	c_test_vertex.bind();
	c_test_index.bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void camera::shutdown_test(void)
{
	c_test_vertex_array.destroy();
	c_test_index.destroy();
	c_test_vertex.destroy();
}

void camera::draw_test(void)
{
	float x, y, z;
	float size = 80.0f;
	float step = 4.0f;
	glm::mat4 m;

	glBindVertexArray(c_test_vertex_array.id());

	for (x = 0.0f; x < size; x += step) {
		for (y = 0.0f; y < size; y += step) {
			for (z = 0.0f; z < size; z += step) {
				m = glm::translate(glm::vec3(x, y, z));
				c_program.uniform(c_uniform_mat4_loc[UNIFORM_MODEL_MATRIX], 1, false, &m);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}
		}
	}

	glBindVertexArray(0);
}

SGE_SCENE_END

