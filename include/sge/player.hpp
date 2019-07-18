//
//
#ifndef SGE_PLAYER_HPP
#define SGE_PLAYER_HPP

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <sge/common.hpp>
#include <sge/subsystem.hpp>
#include <sge/window.hpp>
#include <sge/game.hpp>

SGE_BEGIN

class player: public subsystem {
public:
	player(uv_loop_t *loop = NULL);
	virtual ~player(void);

public:
	window &get_window(void);
	unsigned int fps(void) const;

protected:
	bool init(void) override;
	void shutdown(void) override;
	void handle_event(const SDL_Event &event) override;
	virtual void update(float elapsed);
	virtual void draw(void);

private:
	void frame(void);
	static void frame_cb(uv_timer_t *timer);
	void state(void);
	static void state_cb(uv_timer_t *timer);
	void show_fps(void);

private:
	enum {
		FLAG_WINDOW_VISIBLED = 0x1,
		FLAG_WINDOW_FULLSCREEN = 0x2,
	};

private:
	uv_timer_t m_frame_timer;
	uv_timer_t m_state_timer;
	window m_window;
	ImGuiContext *m_imgui;
	int m_flags;
	uint64_t m_last;
	unsigned int m_fps;
	unsigned int m_fps_count;
	game m_game;
	bool m_show_fps;
};

inline window &player::get_window(void)
{
	return m_window;
}

inline unsigned int player::fps(void) const
{
	return m_fps;
}

SGE_END

#endif // SGE_PLAYER_HPP

