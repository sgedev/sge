//
//
#include <sge/renderer.hpp>

SGE_BEGIN

const char *vertex_shader_source =
	"#version 330\n"
	"uniform mat4 sge_model_matrix;\n"
	"uniform mat4 sge_view_matrix;\n"
	"uniform mat4 sge_projection_matrix;\n"
	"layout (location = 0) in vec4 position;\n"
	"void main() {\n"
	"	gl_Position = sge_projection_matrix * sge_view_matrix * sge_model_matrix * position;\n"
	"}\n"
	;

const char *fragment_shader_source =
	"#version 330\n"
	"void main() {\n"
	"	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
	"}\n"
	;

renderer::renderer(void)
	: m_test_vertex(GL_ARRAY_BUFFER, GL_STATIC_DRAW)
	, m_test_index(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW)
{
}

renderer::~renderer(void)
{
}

bool renderer::init(window *pw)
{
	if (!setup_program()) {
		SGE_LOGE("failed to initialize glsl.\n");
		return false;
	}

	m_uniform_mat4[UNIFORM_MODEL_MATRIX] = glm::mat4(1.0f);
	m_uniform_mat4[UNIFORM_VIEW_MATRIX] = glm::mat4(1.0f);
	m_uniform_mat4[UNIFORM_PROJECTION_MATRIX] = glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 0.5f, 999.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	init_test();

	return true;
}

void renderer::shutdown(void)
{
	shutdown_test();

	m_program.Destroy();
}

void renderer::render(view &v)
{
	m_program.Use();

	m_uniform_mat4[UNIFORM_VIEW_MATRIX] = v.transform();
	m_uniform_mat4[UNIFORM_PROJECTION_MATRIX] = v.projection();

	commit_uniforms();

	draw_test();
}

bool renderer::setup_program(void)
{
	if (!m_program.Create())
		return false;

	SGE_LOGD("building vertex shader...\n");
	if (!m_program.AddShader(GL_VERTEX_SHADER, vertex_shader_source)) {
		SGE_LOGE("failed to build vertex shader: %s\n", m_program.InfoLog());
		m_program.Destroy();
		return false;
	}

	SGE_LOGD("building fragment shader...\n");
	if (!m_program.AddShader(GL_FRAGMENT_SHADER, fragment_shader_source)) {
		SGE_LOGE("failed to build fragment shader: %s\n", m_program.InfoLog());
		m_program.Destroy();
		return false;
	}

	SGE_LOGD("linking program...\n");
	if (!m_program.Link()) {
		SGE_LOGE("failed to link program: %s\n", m_program.InfoLog());
		m_program.Destroy();
		return false;
	}

	// clearing uniform locations...

	for (int i = 0; i < UNIFORM_FLOAT_MAX; ++i)
		m_uniform_float_loc[i] = -1;

	for (int i = 0; i < UNIFORM_MAT4_MAX; ++i)
		m_uniform_mat4_loc[i] = -1;

	// getting uniform locations...
	m_uniform_mat4_loc[UNIFORM_MODEL_MATRIX] = m_program.UniformLocation("sge_model_matrix");
	m_uniform_mat4_loc[UNIFORM_VIEW_MATRIX] = m_program.UniformLocation("sge_view_matrix");
	m_uniform_mat4_loc[UNIFORM_PROJECTION_MATRIX] = m_program.UniformLocation("sge_projection_matrix");

	return true;
}

void renderer::commit_uniforms(void)
{
	// float
	for (int i = 0; i < UNIFORM_FLOAT_MAX; ++i) {
		if (m_uniform_float_loc[i] >= 0)
			m_program.Uniform(m_uniform_float_loc[i], m_uniform_float[i]);
	}

	// mat4
	for (int i = 0; i < UNIFORM_MAT4_MAX; ++i) {
		if (m_uniform_mat4_loc[i] >= 0)
			m_program.Uniform(m_uniform_mat4_loc[i], 1, false, &m_uniform_mat4[i]);
	}
}

void renderer::init_test(void)
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

	m_test_vertex.Create(test_vertex, sizeof(test_vertex));
	m_test_index.Create(test_index, sizeof(test_index));
	m_test_vertex_array.Create();

	m_test_vertex_array.Bind();
	m_test_vertex.Bind();
	m_test_index.Bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void renderer::shutdown_test(void)
{
	m_test_vertex_array.Destroy();
	m_test_index.Destroy();
	m_test_vertex.Destroy();
}

void renderer::draw_test(void)
{
	float x, y, z;
	float size = 50.0f;
	float step = 4.0f;
	glm::mat4 m;

	glBindVertexArray(m_test_vertex_array.Id());

	for (x = 0.0f; x < size; x += step) {
		for (y = 0.0f; y < size; y += step) {
			for (z = 0.0f; z < size; z += step) {
				m = glm::translate(glm::vec3(x, y, z));
				m_program.Uniform(m_uniform_mat4_loc[UNIFORM_MODEL_MATRIX], 1, false, &m);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}
		}
	}

	glBindVertexArray(0);
}

SGE_END

