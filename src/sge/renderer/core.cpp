//
//
#include <GL.hpp>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#define DEBUG_DRAW_IMPLEMENTATION
#include "debug_draw.hpp"

#include <sge/renderer.hpp>

SGE_RENDERER_BEGIN

static SDL_Window *window;
static Uint32 windowId;
static glm::ivec4 windowRect;
static bool isWindowVisibled;
static SDL_GLContext glContext;
static bool isFullscreen;
static ImGuiContext *imContext;

enum UniformFloat {
	UniformElapsed = 0,
	UniformFloatMax
};

enum UniformMat4 {
	UniformModelMatrix = 0,
	UniformViewMatrix,
	UniformProjectionMatrix,
	UniformNormalMatrix,
	UniformMat4Max
};

static GL::Program program;

// float uniforms
static int uniformFloatLoc[UniformFloatMax];
static float uniformFloat[UniformFloatMax];

// mat4 uniforms
static int uniformMat4Loc[UniformMat4Max];
static glm::mat4 uniformMat4[UniformMat4Max];

const char *vertexShaderSource =
	"#version 330\n"
	"uniform mat4 sge_model_matrix;\n"
	"uniform mat4 sge_view_matrix;\n"
	"uniform mat4 sge_projection_matrix;\n"
	"layout (location = 0) in vec4 position;\n"
	"void main() {\n"
	"	gl_Position = sge_projection_matrix * sge_view_matrix * sge_model_matrix * position;\n"
	"}\n"
	;

const char *fragmentShaderSource =
	"#version 330\n"
	"void main() {\n"
	"	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
	"}\n"
	;

static GL::Buffer testVertex(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
static GL::Buffer testIndex(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
static GL::VertexArray testVertexArray;

static void initTest(void)
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

	testVertex.create(test_vertex, sizeof(test_vertex));
	testIndex.create(test_index, sizeof(test_index));
	testVertexArray.create();

	testVertexArray.bind();
	testVertex.bind();
	testIndex.bind();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

static void shutdownTest(void)
{
	testVertexArray.destroy();
	testIndex.destroy();
	testVertex.destroy();
}

static void drawTest(void)
{
	float x, y, z;
	float size = 80.0f;
	float step = 4.0f;
	glm::mat4 m;

	glBindVertexArray(testVertexArray.id());

	for (x = 0.0f; x < size; x += step) {
		for (y = 0.0f; y < size; y += step) {
			for (z = 0.0f; z < size; z += step) {
				m = glm::translate(glm::vec3(x, y, z));
				program.uniform(uniformMat4Loc[UniformModelMatrix], 1, false, &m);
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}
		}
	}

	glBindVertexArray(0);
}

static bool initProgram(void)
{
	if (!program.create())
		return false;

	SGE_LOGD("Building vertex shader...\n");
	if (!program.addShader(GL_VERTEX_SHADER, vertexShaderSource)) {
		SGE_LOGE("Failed to build vertex shader: %s\n", program.infoLog());
		program.destroy();
		return false;
	}

	SGE_LOGD("Building fragment shader...\n");
	if (!program.addShader(GL_FRAGMENT_SHADER, fragmentShaderSource)) {
		SGE_LOGE("Failed to build fragment shader: %s\n", program.infoLog());
		program.destroy();
		return false;
	}

	SGE_LOGD("Linking program...\n");
	if (!program.link()) {
		SGE_LOGE("Failed to link program: %s\n", program.infoLog());
		program.destroy();
		return false;
	}

	// clearing uniform locations...

	for (int i = 0; i < UniformFloatMax; ++i)
		uniformFloatLoc[i] = -1;

	for (int i = 0; i < UniformMat4Max; ++i)
		uniformMat4Loc[i] = -1;

	// getting uniform locations...
	uniformMat4Loc[UniformModelMatrix] = program.uniformLocation("sge_model_matrix");
	uniformMat4Loc[UniformViewMatrix] = program.uniformLocation("sge_view_matrix");
	uniformMat4Loc[UniformProjectionMatrix] = program.uniformLocation("sge_projection_matrix");

	return true;
}

static void commitUniforms(void)
{
	// float
	for (int i = 0; i < UniformFloatMax; ++i) {
		if (uniformFloatLoc[i] >= 0)
			program.uniform(uniformFloatLoc[i], uniformFloat[i]);
	}

	// mat4
	for (int i = 0; i < UniformMat4Max; ++i) {
		if (uniformMat4Loc[i] >= 0)
			program.uniform(uniformMat4Loc[i], 1, false, &uniformMat4[i]);
	}
}

#ifdef SGE_DEBUG

static inline const char *debugSource(GLenum source)
{
	switch (source) {
	case GL_DEBUG_SOURCE_API:             return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "WINDOW_SYSTEM";
	case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER_COMPILER";
	case GL_DEBUG_SOURCE_THIRD_PARTY:     return "THIRD_PART";
	case GL_DEBUG_SOURCE_APPLICATION:     return "APPLICATION";
	case GL_DEBUG_SOURCE_OTHER:           return "OTHER";
	}
	return "UNKNOWN_SOURCE";
}

static inline const char *debugType(GLenum type)
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
	return "UNKNOWN_TYPE";
}

static inline const char *debugSeverity(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:         return "HIGH";
	case GL_DEBUG_SEVERITY_MEDIUM:       return "MEDIUM";
	case GL_DEBUG_SEVERITY_LOW:          return "LOW";
	case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
	}
	return "UNKNOWN_SEVERITY";
}

static void debugOutput(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar *message, const void *data)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		SGE_LOGE("GL-%04d|%s|%s|%s] %s", id,
			debugSource(source), debugType(type), debugSeverity(severity), message);
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
	case GL_DEBUG_TYPE_PORTABILITY:
	case GL_DEBUG_TYPE_PERFORMANCE:
		SGE_LOGW("GL-%04d|%s|%s|%s] %s", id,
			debugSource(source), debugType(type), debugSeverity(severity), message);
		break;
	default:
		SGE_LOGD("GL-%04d|%s|%s|%s] %s", id,
			debugSource(source), debugType(type), debugSeverity(severity), message);
		break;
	}
}

#endif // SGE_DEBUG

bool init(void)
{
	SGE_ASSERT(window == NULL);
	SGE_ASSERT(glContext == NULL);

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

	window = SDL_CreateWindow("SGE",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

	if (window == NULL)
		goto bad0;

	isFullscreen = false;
	windowId = SDL_GetWindowID(window);
	SDL_GetWindowPosition(window, &windowRect[0], &windowRect[1]);
	SDL_GetWindowSize(window, &windowRect[2], &windowRect[3]);

	SGE_LOGD("Initializing OpenGL...\n");

	glContext = SDL_GL_CreateContext(window);
	if (glContext == NULL) {
		SGE_LOGE("Failed to create OpenGL context.\n");
		goto bad1;
	}

	SDL_GL_MakeCurrent(window, glContext);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		SGE_LOGE("Failed to initialize GLEW.\n");
		goto bad2;
	}

	SGE_LOGI("GLEW: %s\n", glewGetString(GLEW_VERSION));
	SGE_LOGI("OpenGL: %s\n", glGetString(GL_VERSION));

#ifdef SGE_DEBUG
	if (GL_KHR_debug) {
		SGE_LOGD("Enable OpenGL debug output.\n");
		glDebugMessageCallback(debugOutput, NULL);
	}
#endif

	if (!initProgram())
		goto bad2;

	imContext = ImGui::CreateContext();
	if (imContext == NULL)
		goto bad3;

	ImGui::SetCurrentContext(imContext);

	SGE_LOGI("ImGui: %s\n", ImGui::GetVersion());

	{
		ImGuiIO &io = ImGui::GetIO();
		io.IniFilename = NULL;

		ImGui::StyleColorsDark();

		ImGuiStyle &style = ImGui::GetStyle();
		style.Alpha = 1.0f;
		style.WindowRounding = 0.0f;
		style.WindowBorderSize = 1.0f;
		style.ChildRounding = 0.0f;
		style.ChildBorderSize = 0.0f;
		style.PopupRounding = 0.0f;
		style.PopupBorderSize = 0.0f;
	}

    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 130");

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	initTest();

	return true;

bad3:
	program.destroy();

bad2:
	SDL_GL_DeleteContext(glContext);
	glContext = NULL;

bad1:
	SDL_DestroyWindow(window);
	window = NULL;
	windowId = 0;

bad0:
	return false;
}

void shutdown(void)
{
	SGE_ASSERT(window != NULL);
	SGE_ASSERT(glContext != NULL);
	SGE_ASSERT(imContext != NULL);

	shutdownTest();

	ImGui::SetCurrentContext(imContext);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::SetCurrentContext(NULL);
	ImGui::DestroyContext(imContext);

	if (glContext == SDL_GL_GetCurrentContext())
		SDL_GL_MakeCurrent(window, NULL);

	SDL_GL_DeleteContext(glContext);
	glContext = NULL;

	SDL_DestroyWindow(window);
	window = NULL;
	windowId = 0;
}

bool handleEvent(const SDL_Event &evt)
{
	SGE_ASSERT(window != NULL);
	SGE_ASSERT(glContext != NULL);

	if (evt.window.windowID != windowId)
		return false;

	ImGui::SetCurrentContext(imContext);
	ImGui_ImplSDL2_ProcessEvent(&evt);

	switch (evt.type) {
	case SDL_WINDOWEVENT:
		switch (evt.window.event) {
		case SDL_WINDOWEVENT_MOVED:
			windowRect.x = evt.window.data1;
			windowRect.y = evt.window.data2;
			return true;
		case SDL_WINDOWEVENT_RESIZED:
			windowRect.z = evt.window.data1;
			windowRect.w = evt.window.data2;
			return true;
		case SDL_WINDOWEVENT_EXPOSED:
		case SDL_WINDOWEVENT_SHOWN:
			isWindowVisibled = true;
			return true;
		case SDL_WINDOWEVENT_HIDDEN:
		case SDL_WINDOWEVENT_MINIMIZED:
			isWindowVisibled = false;
			return true;
		}
		break;
	case SDL_KEYDOWN:
		if (evt.key.keysym.sym == SDLK_RETURN && evt.key.keysym.mod & KMOD_ALT) {
			isFullscreen = !isFullscreen;
			if (isFullscreen)
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			else
				SDL_SetWindowFullscreen(window, 0);
			return true;
		}
		break;
	}

	return false;
}

bool beginFrame(void)
{
	SGE_ASSERT(window != NULL);
	SGE_ASSERT(glContext != NULL);
	SGE_ASSERT(imContext != NULL);

	if (!isWindowVisibled)
		return false;

	if (windowRect[2] < 1 || windowRect[3] < 1)
		return false;

	ImGui::SetCurrentContext(imContext);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();

	ImGui::ShowDemoWindow(NULL);

	return true;
}

void endFrame(void)
{
	SGE_ASSERT(window != NULL);
	SGE_ASSERT(window == SDL_GL_GetCurrentWindow());
	SGE_ASSERT(glContext != NULL);
	SGE_ASSERT(glContext == SDL_GL_GetCurrentContext());
	SGE_ASSERT(imContext != NULL);
	SGE_ASSERT(imContext == ImGui::GetCurrentContext());

	ImGui::Render();

	if (SDL_GL_MakeCurrent(window, glContext))
		return;

	glViewport(0, 0, windowRect[2], windowRect[3]);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program.id());

	// TODO render all world.

	commitUniforms();
	drawTest();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(window);
}

SGE_RENDERER_END

