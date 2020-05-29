//
//
#ifndef SGE_CLIENT_HPP
#define SGE_CLIENT_HPP

#include <string>

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <SGE/Common.hpp>
#include <SGE/Game.hpp>
#include <SGE/Graphics.hpp>

SGE_BEGIN

class Client: public Game {
public:
	Client(uv_loop_t *loop);
	virtual ~Client(void);

public:
	bool start(const std::string &initrc) override;
	void stop(void) override;
	bool handleEvent(const SDL_Event &event) override;
	const std::string &path(void) const;

protected:
	bool init(void) override;
	void shutdown(void) override;
	void frame(float elapsed) override;
	void update(void) override;
	void updateGui(void);
	bool initWindow(void);
	void releaseWindow(void);
	bool handleWindowEvent(const SDL_WindowEvent &event);
	void updateWindow(void);

protected:
	SDL_Window *m_window;
	SDL_GLContext m_context;
	Graphics::Renderer m_renderer;

private:
#if defined SGE_DEBUG && defined SGE_LOG
	static void APIENTRY glDebugOutput(
		GLenum source, GLenum type, unsigned int id, GLenum severity,
		GLsizei length, const char *message, const void *userParam);
#endif

private:
	std::string m_path;
	Uint32 m_id;
	union GL3WProcs m_gl3w;
	glm::ivec4 m_rect;
	bool m_visibled;
	ImGui_ImplSDL2_Context *m_imgui_sdl2;
	ImGui_ImplOpenGL3_Context *m_imgui_opengl3;
	ImGuiContext *m_imgui;
};

SGE_INLINE const std::string &Client::path(void) const
{
	return m_path;
}

SGE_END

#endif // SGE_CLIENT_HPP
