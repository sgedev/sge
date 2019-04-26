//
//
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <sge/gl.hpp>
#include <sge/gui.hpp>

SGE_GUI_BEGIN

static SDL_Window *s_gl_window;

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

	return true;
}

void shutdown(void)
{
	SGE_ASSERT(s_gl_window != NULL);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void handle_event(const SDL_Event &event)
{
	SGE_ASSERT(s_gl_window != NULL);

	ImGui_ImplSDL2_ProcessEvent(&event);
}

void update(void)
{
	SGE_ASSERT(s_gl_window != NULL);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(s_gl_window);
	ImGui::NewFrame();

	node::update_all();

	// ImGui::ShowDemoWindow(NULL);

	ImGui::Render();
}

void draw(void)
{
	SGE_ASSERT(s_gl_window != NULL);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

SGE_GUI_END

