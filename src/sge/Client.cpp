//
//
#include <SGE/Client.hpp>

SGE_BEGIN

Client::Client(uv_loop_t *loop):
	Game(loop),
	m_window(NULL),
	m_context(NULL),
	m_imgui_sdl2(NULL),
	m_imgui_opengl3(NULL),
	m_imgui(NULL)
{
	SGE_ASSERT(loop != NULL);
}

Client::~Client(void)
{
}

bool Client::start(void)
{
	SGE_ASSERT(m_window == NULL);
	SGE_ASSERT(m_context == NULL);
	SGE_ASSERT(m_imgui == NULL);
	SGE_ASSERT(m_imgui_sdl2 == NULL);
	SGE_ASSERT(m_imgui_opengl3 == NULL);

	if (!Game::start())
		return false;

	if (!initWindow()) {
		Game::stop();
		return false;
	}

	// OpenGL

	SDL_GL_MakeCurrent(m_window, m_context);

	if (gl3wInit(&m_gl3w, (GL3WGetProcAddressProc)SDL_GL_GetProcAddress) < 0) {
		logError("Failed to init OpenGL procs.\n");
		releaseWindow();
		Game::stop();
		return false;
	}

	gl3wProcs = &m_gl3w;

	logInfo("OpenGL: %s\n", glGetString(GL_VERSION));
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

#if defined SGE_DEBUG && defined SGE_LOG
	if (GL_KHR_debug)
		glDebugMessageCallback(glDebugOutput, this);
#endif

	// ImGui

	IMGUI_CHECKVERSION();
	m_imgui = ImGui::CreateContext();
	ImGui::SetCurrentContext(m_imgui);

	ImGuiIO &io = ImGui::GetIO();
	io.IniFilename = NULL;
	io.LogFilename = NULL;

	ImGuiStyle &style = ImGui::GetStyle();
	style.WindowRounding = 0.0f;
	
	m_imgui_sdl2 = ImGui_ImplSDL2_NewForOpenGL(m_window, m_context);
	m_imgui_opengl3 = ImGui_ImplOpenGL3_New("#version 130");

	m_renderer.init(scene());

	return true;
}

void Client::stop(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_context != NULL);
	SGE_ASSERT(m_imgui != NULL);
	SGE_ASSERT(m_imgui_sdl2 != NULL);
	SGE_ASSERT(m_imgui_opengl3 != NULL);

	SDL_GL_MakeCurrent(m_window, m_context);

	ImGui_ImplOpenGL3_Destroy(m_imgui_opengl3);
	ImGui_ImplSDL2_Destroy(m_imgui_sdl2);
	ImGui::DestroyContext(m_imgui);

	releaseWindow();

	Game::stop();
}

bool Client::handleEvent(const SDL_Event &event)
{
	if (Game::handleEvent(event))
		return true;

	SGE_ASSERT(m_window != NULL);

	if (event.type == SDL_WINDOWEVENT)
		handleWindowEvent(event.window);

	SGE_ASSERT(m_imgui != NULL);
	SGE_ASSERT(m_imgui_sdl2 != NULL);
	SGE_ASSERT(m_imgui_opengl3 != NULL);

	ImGui::SetCurrentContext(m_imgui);
	ImGui_ImplSDL2_MakeCurrent(m_imgui_sdl2);
	ImGui_ImplOpenGL3_MakeCurrent(m_imgui_opengl3);

	// TODO game input or gui input?
	if (ImGui_ImplSDL2_ProcessEvent(&event))
		return true;

	return false;
}

void Client::frame(float elapsed)
{
	Game::frame(elapsed);

	updateWindow();
}

bool Client::initWindow(void)
{
	SGE_ASSERT(m_window == NULL);
	SGE_ASSERT(m_context == NULL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#ifdef SGE_DEBUG
	int glFlags;
	logInfo("OpenGL debug enabled.\n");
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &glFlags);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, glFlags | SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	logDebug("Creating main window...\n");

	m_window = SDL_CreateWindow("SGE",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	if (m_window == NULL) {
		logError("Failed to create main window.");
		return false;
	}

	m_visibled = true;
	m_id = SDL_GetWindowID(m_window);
	SDL_GetWindowPosition(m_window, &m_rect[0], &m_rect[1]);
	SDL_GetWindowSize(m_window, &m_rect[2], &m_rect[3]);

	logDebug("Create OpenGL context...");
	m_context = SDL_GL_CreateContext(m_window);
	if (m_context == NULL) {
		SDL_DestroyWindow(m_window);
		m_window = NULL;
		logError("Failed to create OpenGL context.");
		return false;
	}

	return true;
}

void Client::releaseWindow(void)
{
	SGE_ASSERT(m_context != NULL);
	SDL_GL_DeleteContext(m_context);
	m_context = NULL;

	SGE_ASSERT(m_window != NULL);
	SDL_DestroyWindow(m_window);
	m_window = NULL;
}

bool Client::handleWindowEvent(const SDL_WindowEvent &event)
{
	if (event.windowID != m_id)
		return false;

	switch (event.event) {
	case SDL_WINDOWEVENT_SHOWN:
		m_visibled = true;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		m_visibled = false;
		break;
	case SDL_WINDOWEVENT_MOVED:
		m_rect[0] = event.data1;
		m_rect[1] = event.data2;
		break;
	case SDL_WINDOWEVENT_RESIZED:
		m_rect[2] = event.data1;
		m_rect[3] = event.data2;
		break;
	case SDL_WINDOWEVENT_FOCUS_GAINED:
		break;
	case SDL_WINDOWEVENT_FOCUS_LOST:
		break;
	default:
		return false;
	}

	return true;
}

void Client::updateWindow(void)
{
	if (!m_visibled)
		return;

	if (m_rect[2] < 1 || m_rect[3] < 1)
		return;

	if (SDL_GL_MakeCurrent(m_window, m_context) < 0)
		return;

	ImGui::SetCurrentContext(m_imgui);

	ImGui_ImplSDL2_MakeCurrent(m_imgui_sdl2);
	ImGui_ImplOpenGL3_MakeCurrent(m_imgui_opengl3);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();
	updateGui();
	ImGui::Render();

	glClear(GL_COLOR_BUFFER_BIT);
	m_renderer.draw();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(m_window);
}

void Client::updateGui(void)
{
}

#if defined SGE_DEBUG && defined SGE_LOG
void APIENTRY Client::glDebugOutput(
	GLenum source, GLenum type, unsigned int id, GLenum severity,
	GLsizei length, const char *message, const void *userParam)
{
	const char *sourceStr;
    switch (source) {
	case GL_DEBUG_SOURCE_API:             sourceStr = "API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sourceStr = "WINDOW_SYSTEM"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "SHADER_COMPILER"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     sourceStr = "THIRD_PART"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     sourceStr = "APPLICATION"; break;
	default:
	case GL_DEBUG_SOURCE_OTHER:           sourceStr = "OTHER"; break;
    }

	const char *typeStr;
    switch (type) {
	case GL_DEBUG_TYPE_ERROR:               typeStr = "ERROR"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "DEPRECATED_BEHAVIOR"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeStr = "UNDEFINED_BEHAVIOR"; break; 
	case GL_DEBUG_TYPE_PORTABILITY:         typeStr = "PORTABILITY"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         typeStr = "PERFORMANCE"; break;
	case GL_DEBUG_TYPE_MARKER:              typeStr = "MARKER"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          typeStr = "PUSH_GROUP"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           typeStr = "POP_GROUP"; break;
	default:
	case GL_DEBUG_TYPE_OTHER:               typeStr = "OTHER"; break;
    }
    
	const char *severityStr;
    switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:         severityStr = "H"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       severityStr = "M"; break;
	case GL_DEBUG_SEVERITY_LOW:          severityStr = "L"; break;
	default:
	case GL_DEBUG_SEVERITY_NOTIFICATION: severityStr = "N"; break;
    };

	if (type == GL_DEBUG_TYPE_ERROR)
		logError("GL/%s/%s/%s: %s", severityStr, sourceStr, typeStr, message);
	else
		logDebug("GL/%s/%s/%s: %s", severityStr, sourceStr, typeStr, message);
}
#endif

SGE_END
