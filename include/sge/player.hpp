//
//
#ifndef SGE_PLAYER_HPP
#define SGE_PLAYER_HPP

#include <GLEX/glex.h>
#include <imgui.h>

#include <sge/common.hpp>
#include <sge/subsystem.hpp>
#include <sge/game.hpp>

SGE_BEGIN

class player: public subsystem {
public:
	player(uv_loop_t *loop);
	virtual ~player(void);

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
	game m_game;

private:
	enum {
		FLAG_VISIBLED = 0x1,
		FLAG_FULLSCREEN = 0x2,
	};

private:
	SDL_Window *m_window;
	glm::ivec4 m_rect;
	Uint32 m_id;
	SDL_GLContext m_gl;
	ImGuiContext *m_imgui;
	GLEXContext *m_glex;
	int m_flags;
	std::string m_title;
	uv_timer_t m_frame_timer;
	uv_timer_t m_state_timer;
	uint64_t m_last;
	int m_fps;
	int m_fps_count;
};

SGE_END

#endif // SGE_PLAYER_HPP

