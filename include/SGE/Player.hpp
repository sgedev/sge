//
//
#ifndef SGE_PLAYER_HPP
#define SGE_PLAYER_HPP

#include <string>

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

#include <SGE/Handle.hpp>
#include <SGE/Scene.hpp>
#include <SGE/Graphics.hpp>
#include <SGE/Physics.hpp>

SGE_BEGIN

class Player: public Handle {
public:
	Player(uv_loop_t *loop);
	virtual ~Player(void);

public:
	const std::string &path(void) const;
	bool start(const std::string &path) override;
	void stop(void) override;
	bool handleEvent(const SDL_Event &event) override;

protected:
	bool initWindow(void);
	void releaseWindow(void);
	bool handleWindowEvent(const SDL_WindowEvent &event);
	void updateWindow(void);
	virtual void updateGui(void);

private:
	virtual void frame(void);
	static void frameCallback(uv_timer_t *p);
	void state(void);
	static void stateCallback(uv_timer_t *p);

protected:
	SDL_Window *m_window;
	SDL_GLContext m_context;
	Scene m_scene;
	Graphics::Renderer m_renderer;
	Physics::World m_physicsWorld;

private:
	uv_timer_t m_frameTimer;
	uv_timer_t m_stateTimer;
	int m_fps;
	int m_fpsCount;
	Uint32 m_last;
	std::string m_path;
	Uint32 m_id;
	union GL3WProcs m_gl3w;
	glm::ivec4 m_rect;
	bool m_visibled;
	ImGui_ImplSDL2_Context *m_imgui_sdl2;
	ImGui_ImplOpenGL3_Context *m_imgui_opengl3;
	ImGuiContext *m_imgui;
};

SGE_INLINE const std::string &Player::path(void) const
{
	return m_path;
}

SGE_END

#endif // SGE_PLAYER_HPP
