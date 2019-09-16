//
//
#ifndef SGE_PLAYER_HPP
#define SGE_PLAYER_HPP

#include <GL/glex.h>

#include <imgui.h>

#include <sge/common.hpp>
#include <sge/subsystem.hpp>
#include <sge/window.hpp>
#include <sge/game.hpp>

SGE_BEGIN

class player: public subsystem {
public:
	player(uv_loop_t *loop);
	virtual ~player(void);

public:
	unsigned int fps(void) const;

protected:
	bool init(void) override;
	void shutdown(void) override;
	bool handle_event(const SDL_Event *event) override;

protected:
	virtual void update(float elapsed);

private:
	void render(void);
	static void frame_cb(uv_timer_t *p);
	void frame(void);
	static void state_cb(uv_timer_t* p);
	void state(void);

protected:
	window m_window;
	game m_game;

private:
	enum {
		FLAG_VISIBLED = 0x1,
		FLAG_FULLSCREEN = 0x2,
	};

private:
	ImGuiContext *m_imgui;
	GLEXContext *m_glex;
	uv_timer_t m_frame_timer;
	uv_timer_t m_state_timer;
	uint64_t m_last;
	int m_fps;
	int m_fps_count;
};

inline unsigned int player::fps(void) const
{
	return m_fps;
}

SGE_END

#endif // SGE_PLAYER_HPP

