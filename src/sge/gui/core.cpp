//
//
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <sge/gl.hpp>
#include <sge/gui.hpp>

SGE_GUI_BEGIN

static SDL_Window *s_gl_window;
static window s_root;

bool init(void)
{
	s_gl_window = gl::window();
	if (s_gl_window == NULL)
		return false;

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
	io.IniFilename = NULL;

	SGE_LOGI("ImGui: %s\n", ImGui::GetVersion());

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(s_gl_window, gl::context());
    ImGui_ImplOpenGL3_Init("#version 130");

	const glm::ivec2 &size = gl::window_size();
	s_root.create("DESKTOP", 0, 0, size[0], size[1],
		window::FLAG_NO_TITLE |
		window::FLAG_NO_MOVE |
		window::FLAG_NO_RESIZE |
		window::FLAG_NO_COLLAPSE |
		window::FLAG_NO_BACKGROUND |
		window::FLAG_NO_SCROLL |
		window::FLAG_NO_FRONT |
		window::FLAG_VISIBLED);

	return true;
}

void shutdown(void)
{
	SGE_ASSERT(s_gl_window != NULL);

	s_root.destroy();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void handle_event(const SDL_Event &event)
{
	SGE_ASSERT(s_gl_window != NULL);

	ImGui_ImplSDL2_ProcessEvent(&event);

	const glm::ivec2 size = gl::window_size();
	s_root.resize(size[0], size[1]);
}

void update(float elapsed)
{
	SGE_ASSERT(s_gl_window != NULL);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(s_gl_window);
	ImGui::NewFrame();

	ImGui::Begin("SGE", NULL,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoBringToFrontOnFocus);

	ImGui::SetWindowPos(ImVec2(0, 0));

	const glm::ivec2 &size = gl::window_size();
	ImGui::SetWindowSize(ImVec2(size[0], size[1]));

	ImGui::Text("fps %d\n", fps());

	s_root.update(elapsed);

	ImGui::End();

	ImGui::ShowDemoWindow(NULL);

	ImGui::Render();
}

void draw(void)
{
	SGE_ASSERT(s_gl_window != NULL);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

window *root(void)
{
	SGE_ASSERT(s_gl_window != NULL);

	return &s_root;
}

SGE_GUI_END
