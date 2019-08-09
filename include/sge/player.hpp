//
//
#ifndef SGE_PLAYER_HPP
#define SGE_PLAYER_HPP

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <sge/common.hpp>
#include <sge/window.hpp>
#include <sge/scene.hpp>
#include <sge/view.hpp>
#include <sge/renderer.hpp>
#include <sge/game.hpp>

SGE_BEGIN

class player: public game::traps {
public:
	player(uv_loop_t *loop);
	virtual ~player(void);

public:
	bool start(void);
	void stop(void);
	void feed_event(const SDL_Event &event);
	bool started(void) const;
	unsigned int fps(void) const;
	window &main_window(void);
	game &current_game(void);

protected:
	virtual bool init(void);
	virtual void shutdown(void);
	virtual void handle_event(const SDL_Event &event);
	virtual void update(float elapsed);

protected: // game traps
	unsigned int trap_fps(void) override;

private:
	bool init_imgui(void);
	void shutdown_imgui(void);
	void draw_loading(void);
	void draw_ready(void);
	void draw_fps(void);
	void draw_hud(void);
	void frame(void);
	static void frame_cb(uv_timer_t *timer);
	void state(void);
	static void state_cb(uv_timer_t *timer);

private:
	enum {
		FLAG_STARTED = 0x1,
		FLAG_WINDOW_VISIBLED = 0x2,
		FLAG_WINDOW_FULLSCREEN = 0x4,
	};

private:
	uv_loop_t *m_loop;
	game m_game;
	uv_timer_t m_frame_timer;
	uv_timer_t m_state_timer;
	int m_flags;
	uint64_t m_last;
	unsigned int m_fps;
	unsigned int m_fps_count;
	window m_main_window;
	renderer m_renderer;
	view m_view;
	ImGuiContext *m_imgui;
	bool m_draw_fps;
	bool m_draw_hud;
};

inline bool player::started(void) const
{
	return m_flags & FLAG_STARTED;
}

inline unsigned int player::fps(void) const
{
	return m_fps;
}

inline window &player::main_window(void)
{
	return m_main_window;
}

inline game &player::current_game(void)
{
	return m_game;
}

SGE_END

#endif // SGE_GAME_HPP

