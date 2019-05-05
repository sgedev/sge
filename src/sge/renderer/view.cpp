//
//
#include <sge/renderer/view.hpp>

#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

SGE_RENDERER_BEGIN

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

view::view(void)
	: m_gl_context(NULL)
	, m_test_vertex(GL_ARRAY_BUFFER, GL_STATIC_DRAW)
	, m_test_index(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW)
{
}

view::~view(void)
{
}

void view::init_test(void)
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

	m_test_vertex.create(test_vertex, sizeof(test_vertex));
	m_test_index.create(test_index, sizeof(test_index));
	m_test_vertex_array.create();

	m_test_vertex_array.bind();
	m_test_vertex.bind();
	m_test_index.bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void view::shutdown_test(void)
{
	m_test_vertex_array.destroy();
	m_test_index.destroy();
	m_test_vertex.destroy();
}

void view::draw_test(void)
{
	float x, y, z;
	float size = 50.0f;
	float step = 3.0f;
	glm::mat4 m;

	glBindVertexArray(m_test_vertex_array.id());

	for (x = 0.0f; x < size; x += step) {
		for (y = 0.0f; y < size; y += step) {
			for (z = 0.0f; z < size; z += step) {
				m = glm::translate(glm::vec3(x, y, z));
				m_program.uniform(m_uniform_mat4_loc[UNIFORM_MODEL_MATRIX], 1, false, &m);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}
		}
	}

	glBindVertexArray(0);
}

bool view::init_program(void)
{
	if (!m_program.create())
		return false;

	SGE_LOGD("Building vertex shader...\n");
	if (!m_program.add_shader(GL_VERTEX_SHADER, vertex_shader_source)) {
		SGE_LOGE("Failed to build vertex shader: %s\n", m_program.info_log());
		m_program.destroy();
		return false;
	}

	SGE_LOGD("Building fragment shader...\n");
	if (!m_program.add_shader(GL_FRAGMENT_SHADER, fragment_shader_source)) {
		SGE_LOGE("Failed to build fragment shader: %s\n", m_program.info_log());
		m_program.destroy();
		return false;
	}

	SGE_LOGD("Linking program...\n");
	if (!m_program.link()) {
		SGE_LOGE("Failed to link program: %s\n", m_program.info_log());
		m_program.destroy();
		return false;
	}

	// clearing uniform locations...

	for (int i = 0; i < UNIFORM_FLOAT_MAX; ++i)
		m_uniform_float_loc[i] = -1;

	for (int i = 0; i < UNIFORM_MAT4_MAX; ++i)
		m_uniform_mat4_loc[i] = -1;

	// getting uniform locations...
	m_uniform_mat4_loc[UNIFORM_MODEL_MATRIX] = m_program.uniform_location("sge_model_matrix");
	m_uniform_mat4_loc[UNIFORM_VIEW_MATRIX] = m_program.uniform_location("sge_view_matrix");
	m_uniform_mat4_loc[UNIFORM_PROJECTION_MATRIX] = m_program.uniform_location("sge_projection_matrix");

	return true;
}

void view::commit_uniforms(void)
{
	// float
	for (int i = 0; i < UNIFORM_FLOAT_MAX; ++i) {
		if (m_uniform_float_loc[i] >= 0)
			m_program.uniform(m_uniform_float_loc[i], m_uniform_float[i]);
	}

	// mat4
	for (int i = 0; i < UNIFORM_MAT4_MAX; ++i) {
		if (m_uniform_mat4_loc[i] >= 0)
			m_program.uniform(m_uniform_mat4_loc[i], 1, false, &m_uniform_mat4[i]);
	}
}

bool view::create(gl::context &gl_context)
{
	SGE_ASSERT(m_gl_context == NULL);

	IMGUI_CHECKVERSION();

    m_imgui_context = ImGui::CreateContext();
	if (m_imgui_context == NULL) {
		SGE_LOGE("Failed to initialize ImGui.\n");
		return false;
	}

	ImGui::SetCurrentContext(m_imgui_context);

    ImGuiIO &io = ImGui::GetIO();
	io.IniFilename = NULL;

	SGE_LOGI("ImGui: %s\n", ImGui::GetVersion());

    ImGui::StyleColorsDark();

	gl_context.make_current();

    ImGui_ImplSDL2_InitForOpenGL(gl_context.get_window()->to_sdl_window(), gl_context.to_sdl_gl_context());
    ImGui_ImplOpenGL3_Init("#version 130");

	if (!init_program()) {
		SGE_LOGE("Failed to initialize ImGui.\n");
		ImGui::DestroyContext(m_imgui_context);
		m_imgui_context = NULL;
		return false;
	}

	m_uniform_mat4[UNIFORM_MODEL_MATRIX] = glm::mat4(1.0f);
	m_uniform_mat4[UNIFORM_VIEW_MATRIX] = glm::mat4(1.0f);
	m_uniform_mat4[UNIFORM_PROJECTION_MATRIX] = glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 0.5f, 999.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	init_test();

	m_gl_context = &gl_context;
	m_window = gl_context.get_window()->to_sdl_window();

	return true;
}

void view::destroy(void)
{
	SGE_ASSERT(m_gl_context != NULL);
	SGE_ASSERT(m_imgui_context != NULL);

	shutdown_test();

	m_program.destroy();

	ImGui::SetCurrentContext(m_imgui_context);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::SetCurrentContext(NULL);
	ImGui::DestroyContext(m_imgui_context);
	m_imgui_context = NULL;

	m_gl_context = NULL;
}

void view::reset(void)
{
	SGE_ASSERT(m_gl_context != NULL);
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_imgui_context != NULL);

	ImGui::SetCurrentContext(m_imgui_context);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();
}

void view::render(void)
{
	SGE_ASSERT(m_gl_context != NULL);
	SGE_ASSERT(m_gl_context == gl::context::current());
	SGE_ASSERT(m_imgui_context != NULL);

	ImGui::Render();

	glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
	glScissor(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);

	if (m_clear_enabled) {
		glClearColor(m_clear_color.r, m_clear_color.g, m_clear_color.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	m_program.use();

	commit_uniforms();

	draw_test();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGui::SetCurrentContext(NULL);
}

SGE_RENDERER_END
