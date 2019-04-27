//
//
#include <sge/gl.hpp>
#include <sge/scene/camera.hpp>

SGE_SCENE_CAMERA_BEGIN

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

static bool s_drawing;
static gl::program s_program;

// float uniforms
static int s_uniform_float_loc[UNIFORM_FLOAT_MAX];
static float s_uniform_float[UNIFORM_FLOAT_MAX];

// mat4 uniforms
static int s_uniform_mat4_loc[UNIFORM_MAT4_MAX];
static glm::mat4 s_uniform_mat4[UNIFORM_MAT4_MAX];

const char *s_vertex_shader_source =
	"#version 330\n"
	"uniform mat4 sge_model_matrix;\n"
	"uniform mat4 sge_view_matrix;\n"
	"uniform mat4 sge_projection_matrix;\n"
	"layout (location = 0) in vec4 position;\n"
	"void main() {\n"
	"	gl_Position = sge_projection_matrix * sge_view_matrix * sge_model_matrix * position;\n"
	"}\n"
	;

const char *s_fragment_shader_source =
	"#version 330\n"
	"out vec4 gl_FragColor;\n"
	"void main() {\n"
	"	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
	"}\n"
	;

static gl::buffer s_test_vertex(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
static gl::buffer s_test_index(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
static gl::vertex_array s_test_vertex_array;

static void init_test(void)
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

	s_test_vertex.create(test_vertex, sizeof(test_vertex));
	s_test_index.create(test_index, sizeof(test_index));
	s_test_vertex_array.create();

	s_test_vertex_array.bind();
	s_test_vertex.bind();
	s_test_index.bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

static void shutdown_test(void)
{
	s_test_vertex_array.destroy();
	s_test_index.destroy();
	s_test_vertex.destroy();
}

static void draw_test(void)
{
	float x, y, z;
	float size = 80.0f;
	float step = 4.0f;
	glm::mat4 m;

	glBindVertexArray(s_test_vertex_array.id());

	for (x = 0.0f; x < size; x += step) {
		for (y = 0.0f; y < size; y += step) {
			for (z = 0.0f; z < size; z += step) {
				m = glm::translate(glm::vec3(x, y, z));
				s_program.uniform(s_uniform_mat4_loc[UNIFORM_MODEL_MATRIX], 1, false, &m);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}
		}
	}

	glBindVertexArray(0);
}

static bool init_program(void)
{
	if (!s_program.create())
		return false;

	SGE_LOGD("Building vertex shader...\n");
	if (!s_program.add_shader(GL_VERTEX_SHADER, s_vertex_shader_source)) {
		SGE_LOGE("Failed to build vertex shader: %s\n", s_program.info_log());
		s_program.destroy();
		return false;
	}

	SGE_LOGD("Building fragment shader...\n");
	if (!s_program.add_shader(GL_FRAGMENT_SHADER, s_fragment_shader_source)) {
		SGE_LOGE("Failed to build fragment shader: %s\n", s_program.info_log());
		s_program.destroy();
		return false;
	}

	SGE_LOGD("Linking program...\n");
	if (!s_program.link()) {
		SGE_LOGE("Failed to link program: %s\n", s_program.info_log());
		s_program.destroy();
		return false;
	}

	// clearing uniform locations...

	for (int i = 0; i < UNIFORM_FLOAT_MAX; ++i)
		s_uniform_float_loc[i] = -1;

	for (int i = 0; i < UNIFORM_MAT4_MAX; ++i)
		s_uniform_mat4_loc[i] = -1;

	// getting uniform locations...
	s_uniform_mat4_loc[UNIFORM_MODEL_MATRIX] = s_program.uniform_location("sge_model_matrix");
	s_uniform_mat4_loc[UNIFORM_VIEW_MATRIX] = s_program.uniform_location("sge_view_matrix");
	s_uniform_mat4_loc[UNIFORM_PROJECTION_MATRIX] = s_program.uniform_location("sge_projection_matrix");

	return true;
}

static void commit_uniforms(void)
{
	// float
	for (int i = 0; i < UNIFORM_FLOAT_MAX; ++i) {
		if (s_uniform_float_loc[i] >= 0)
			s_program.uniform(s_uniform_float_loc[i], s_uniform_float[i]);
	}

	// mat4
	for (int i = 0; i < UNIFORM_MAT4_MAX; ++i) {
		if (s_uniform_mat4_loc[i] >= 0)
			s_program.uniform(s_uniform_mat4_loc[i], 1, false, &s_uniform_mat4[i]);
	}
}

bool init(void)
{
	if (!init_program())
		return false;

	s_uniform_mat4[UNIFORM_MODEL_MATRIX] = glm::mat4(1.0f);
	s_uniform_mat4[UNIFORM_VIEW_MATRIX] = glm::mat4(1.0f);
	s_uniform_mat4[UNIFORM_PROJECTION_MATRIX] = glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 0.5f, 999.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	init_test();

	s_drawing = false;

	return true;
}

void shutdown(void)
{
	SGE_ASSERT(!s_drawing);

	shutdown_test();

	s_program.destroy();
}

void clear(void)
{
	SGE_ASSERT(!s_drawing);

	s_drawing = true;
}

void render(void)
{
	SGE_ASSERT(s_drawing);

	s_drawing = false;

	s_program.use();

	commit_uniforms();

	draw_test();
}

void set_transform(const glm::mat4 &v)
{
	s_uniform_mat4[UNIFORM_VIEW_MATRIX] = v;
}

void set_projection(const glm::mat4 &v)
{
	s_uniform_mat4[UNIFORM_PROJECTION_MATRIX] = v;
}

SGE_SCENE_CAMERA_END

