//
//
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <sge/renderer.hpp>

SGE_RENDERER_BEGIN

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

static SDL_Window *s_window = NULL;
static Uint32 s_window_id = 0;
static int s_window_rect[4];
static bool s_window_visibled = false;
static SDL_GLContext s_gl_context = NULL;
static ImGuiContext *s_imgui_context = NULL;
static GL::Program s_program;

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
	"void main() {\n"
	"	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
	"}\n"
	;

static GL::Buffer s_test_vertex(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
static GL::Buffer s_test_index(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
static GL::VertexArray s_test_vertex_array;

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

	s_test_vertex.Create(test_vertex, sizeof(test_vertex));
	s_test_index.Create(test_index, sizeof(test_index));
	s_test_vertex_array.Create();

	s_test_vertex_array.Bind();
	s_test_vertex.Bind();
	s_test_index.Bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

static void shutdown_test(void)
{
	s_test_vertex_array.Destroy();
	s_test_index.Destroy();
	s_test_vertex.Destroy();
}

static void draw_test(void)
{
	float x, y, z;
	float size = 80.0f;
	float step = 4.0f;
	glm::mat4 m;

	glBindVertexArray(s_test_vertex_array.Id());

	for (x = 0.0f; x < size; x += step) {
		for (y = 0.0f; y < size; y += step) {
			for (z = 0.0f; z < size; z += step) {
				m = glm::translate(glm::vec3(x, y, z));
				s_program.Uniform(s_uniform_mat4_loc[UNIFORM_MODEL_MATRIX], 1, false, &m);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}
		}
	}

	glBindVertexArray(0);
}

static bool init_program(void)
{
	if (!s_program.Create())
		return false;

	SGE_LOGD("Building vertex shader...\n");
	if (!s_program.AddShader(GL_VERTEX_SHADER, s_vertex_shader_source)) {
		SGE_LOGE("Failed to build vertex shader: %s\n", s_program.InfoLog());
		s_program.Destroy();
		return false;
	}

	SGE_LOGD("Building fragment shader...\n");
	if (!s_program.AddShader(GL_FRAGMENT_SHADER, s_fragment_shader_source)) {
		SGE_LOGE("Failed to build fragment shader: %s\n", s_program.InfoLog());
		s_program.Destroy();
		return false;
	}

	SGE_LOGD("Linking program...\n");
	if (!s_program.Link()) {
		SGE_LOGE("Failed to link program: %s\n", s_program.InfoLog());
		s_program.Destroy();
		return false;
	}

	// clearing uniform locations...

	for (int i = 0; i < UNIFORM_FLOAT_MAX; ++i)
		s_uniform_float_loc[i] = -1;

	for (int i = 0; i < UNIFORM_MAT4_MAX; ++i)
		s_uniform_mat4_loc[i] = -1;

	// getting uniform locations...
	s_uniform_mat4_loc[UNIFORM_MODEL_MATRIX] = s_program.UniformLocation("sge_model_matrix");
	s_uniform_mat4_loc[UNIFORM_VIEW_MATRIX] = s_program.UniformLocation("sge_view_matrix");
	s_uniform_mat4_loc[UNIFORM_PROJECTION_MATRIX] = s_program.UniformLocation("sge_projection_matrix");

	return true;
}

static void commit_uniforms(void)
{
	// float
	for (int i = 0; i < UNIFORM_FLOAT_MAX; ++i) {
		if (s_uniform_float_loc[i] >= 0)
			s_program.Uniform(s_uniform_float_loc[i], s_uniform_float[i]);
	}

	// mat4
	for (int i = 0; i < UNIFORM_MAT4_MAX; ++i) {
		if (s_uniform_mat4_loc[i] >= 0)
			s_program.Uniform(s_uniform_mat4_loc[i], 1, false, &s_uniform_mat4[i]);
	}
}

#ifdef SGE_DEBUG

static inline const char *gl_debug_source(GLenum source)
{
	switch (source) {
	case GL_DEBUG_SOURCE_API:             return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "WINDOW_SYSTEM";
	case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER_COMPILER";
	case GL_DEBUG_SOURCE_THIRD_PARTY:     return "THIRD_PART";
	case GL_DEBUG_SOURCE_APPLICATION:     return "APPLICATION";
	case GL_DEBUG_SOURCE_OTHER:           return "OTHER";
	}
	return "";
}

static inline const char *gl_debug_type(GLenum type)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:               return "ERROR";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "UNDEFINED_BEHAVIOR";
	case GL_DEBUG_TYPE_PORTABILITY:         return "PORTABILITY";
	case GL_DEBUG_TYPE_PERFORMANCE:         return "PERFORMANCE";
	case GL_DEBUG_TYPE_MARKER:              return "MARKER";
	case GL_DEBUG_TYPE_PUSH_GROUP:          return "PUSH_GROUP";
	case GL_DEBUG_TYPE_POP_GROUP:           return "POP_GROUP";
	case GL_DEBUG_TYPE_OTHER:               return "OTHER";
	}
	return "";
}

static inline const char *gl_debug_severity(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:         return "HIGH";
	case GL_DEBUG_SEVERITY_MEDIUM:       return "MEDIUM";
	case GL_DEBUG_SEVERITY_LOW:          return "LOW";
	case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
	}
	return "";
}

static void gl_debug_output(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar *message, const void *data)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		SGE_LOGE("GL-%04d|%s|%s|%s] %s", id,
			gl_debug_source(source), gl_debug_type(type), gl_debug_severity(severity), message);
		break;
	default:
		SGE_LOGI("GL-%04d|%s|%s|%s] %s", id,
			gl_debug_source(source), gl_debug_type(type), gl_debug_severity(severity), message);
		break;
	}
}

#endif // SGE_DEBUG

bool init(void)
{
	SGE_ASSERT(s_window == NULL);
	SGE_ASSERT(s_gl_context == NULL);

	SGE_LOGD("Creating OpenGL window...\n");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

#ifdef SGE_DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	s_window = SDL_CreateWindow("SGE",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 300,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

	if (s_window == NULL)
		return false;

	s_window_id = SDL_GetWindowID(s_window);
	SDL_GetWindowPosition(s_window, &s_window_rect[0], &s_window_rect[1]);
	SDL_GetWindowSize(s_window, &s_window_rect[2], &s_window_rect[3]);

	SGE_LOGD("Initializing OpenGL...\n");

	s_gl_context = SDL_GL_CreateContext(s_window);
	if (s_gl_context == NULL) {
		SGE_LOGE("Failed to create OpenGL context.\n");
		SDL_DestroyWindow(s_window);
		s_window = NULL;
		return false;
	}

	SDL_GL_MakeCurrent(s_window, s_gl_context);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		SGE_LOGE("Failed to initialize GLEW.\n");
		SDL_GL_DeleteContext(s_gl_context);
		s_gl_context = NULL;
		SDL_DestroyWindow(s_window);
		s_window = NULL;
		return false;
	}

#ifdef SGE_DEBUG
	if (GL_KHR_debug) {
		SGE_LOGD("Enable OpenGL debug output.\n");
		glDebugMessageCallback(gl_debug_output, NULL);
	}
#endif

	SGE_LOGI("GLEW: %s\n", glewGetString(GLEW_VERSION));
	SGE_LOGI("OpenGL: %s\n", glGetString(GL_VERSION));

	IMGUI_CHECKVERSION();

    s_imgui_context = ImGui::CreateContext();
	if (s_imgui_context == NULL) {
		SGE_LOGE("Failed to initialize ImGui.\n");
		SDL_GL_DeleteContext(s_gl_context);
		s_gl_context = NULL;
		SDL_DestroyWindow(s_window);
		s_window = NULL;
		return false;
	}

	ImGui::SetCurrentContext(s_imgui_context);

    ImGuiIO &io = ImGui::GetIO();
	io.IniFilename = NULL;

	SGE_LOGI("ImGui: %s\n", ImGui::GetVersion());

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(s_window, s_gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");

	if (!init_program()) {
		SGE_LOGE("Failed to initialize ImGui.\n");
		SDL_GL_DeleteContext(s_gl_context);
		s_gl_context = NULL;
		SDL_DestroyWindow(s_window);
		s_window = NULL;
		return false;
	}

	s_uniform_mat4[UNIFORM_MODEL_MATRIX] = glm::mat4(1.0f);
	s_uniform_mat4[UNIFORM_VIEW_MATRIX] = glm::mat4(1.0f);
	s_uniform_mat4[UNIFORM_PROJECTION_MATRIX] = glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 0.5f, 999.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	init_test();

	return true;
}

void shutdown(void)
{
	SGE_ASSERT(s_window != NULL);
	SGE_ASSERT(s_gl_context != NULL);
	SGE_ASSERT(s_imgui_context != NULL);

	shutdown_test();

	s_program.Destroy();

	ImGui::SetCurrentContext(s_imgui_context);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::SetCurrentContext(NULL);
	ImGui::DestroyContext(s_imgui_context);
	s_imgui_context = NULL;

	if (s_gl_context == SDL_GL_GetCurrentContext())
		SDL_GL_MakeCurrent(s_window, NULL);

	SDL_GL_DeleteContext(s_gl_context);
	s_gl_context = NULL;

	SDL_DestroyWindow(s_window);
	s_window = NULL;
	s_window_id = 0;
}

bool handle_event(const SDL_Event &event)
{
	SGE_ASSERT(s_window != NULL);
	SGE_ASSERT(s_imgui_context != NULL);

	ImGui::SetCurrentContext(s_imgui_context);
	ImGui_ImplSDL2_ProcessEvent(&event);
	ImGui::SetCurrentContext(NULL);

	if (event.type != SDL_WINDOWEVENT)
		return false;

	if (event.window.windowID != s_window_id)
		return false;

	switch (event.window.event) {
	case SDL_WINDOWEVENT_MOVED:
		s_window_rect[0] = event.window.data1;
		s_window_rect[1] = event.window.data2;
		return true;
	case SDL_WINDOWEVENT_RESIZED:
		s_window_rect[2] = event.window.data1;
		s_window_rect[3] = event.window.data2;
		return true;
	case SDL_WINDOWEVENT_EXPOSED:
	case SDL_WINDOWEVENT_SHOWN:
		s_window_visibled = true;
		return true;
	case SDL_WINDOWEVENT_HIDDEN:
	case SDL_WINDOWEVENT_MINIMIZED:
		s_window_visibled = false;
		return true;
	}

	return false;
}

SDL_Window *window(void)
{
	SGE_ASSERT(s_window != NULL);

	return s_window;
}

Uint32 window_id(void)
{
	SGE_ASSERT(s_window != NULL);

	return s_window_id;
}

glm::ivec2 window_pos(void)
{
	SGE_ASSERT(s_window != NULL);

	return glm::ivec2(s_window_rect[0], s_window_rect[1]);
}

glm::ivec2 window_size(void)
{
	SGE_ASSERT(s_window != NULL);

	return glm::ivec2(s_window_rect[2], s_window_rect[3]);
}

glm::ivec4 window_rect(void)
{
	SGE_ASSERT(s_window != NULL);

	return glm::ivec4(s_window_rect[0], s_window_rect[1], s_window_rect[2], s_window_rect[3]);
}

SDL_GLContext gl_context(void)
{
	SGE_ASSERT(s_window != NULL);
	SGE_ASSERT(s_gl_context != NULL);

	return s_gl_context;
}

void set_view_matrix(const glm::mat4 &v)
{
	s_uniform_mat4[UNIFORM_VIEW_MATRIX] = v;
}

void set_projection_matrix(const glm::mat4 &v)
{
	s_uniform_mat4[UNIFORM_PROJECTION_MATRIX] = v;
}

bool begin(void)
{
	SGE_ASSERT(s_window != NULL);
	SGE_ASSERT(s_gl_context != NULL);
	SGE_ASSERT(s_imgui_context != NULL);

	if (!s_window_visibled)
		return false;

	ImGui::SetCurrentContext(s_imgui_context);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(s_window);
	ImGui::NewFrame();

	return true;
}

void end(void)
{
	SGE_ASSERT(s_window != NULL);
	SGE_ASSERT(s_gl_context != NULL);

	ImGui::Render();

	SDL_GL_MakeCurrent(s_window, s_gl_context);

	glViewport(0, 0, s_window_rect[2], s_window_rect[3]);
	glClear(GL_COLOR_BUFFER_BIT);

	s_program.Use();

	commit_uniforms();

	draw_test();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGui::SetCurrentContext(NULL);

	SDL_GL_SwapWindow(s_window);
}

SGE_RENDERER_END

