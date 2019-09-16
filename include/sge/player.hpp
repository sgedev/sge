//
//
#ifndef SGE_PLAYER_HPP
#define SGE_PLAYER_HPP

#include <sge/common.hpp>
#include <sge/subsystem.hpp>
#include <sge/renderer.hpp>
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
	virtual void draw(void);

private:
	static void frame_cb(uv_timer_t *p);
	void frame(void);
	static void state_cb(uv_timer_t* p);
	void state(void);

protected:
	game m_game;

private:
	uv_timer_t m_frame_timer;
	uv_timer_t m_state_timer;
	uint64_t m_last;
	int m_fps;
	int m_fps_count;
	renderer m_renderer;
};

SGE_END

#endif // SGE_PLAYER_HPP

