//
//
#ifndef SGE_ENGINE_HPP
#define SGE_ENGINE_HPP

#include <sge/common.hpp>
#include <sge/noncopyable.hpp>

SGE_BEGIN

class engine: public noncopyable {
public:
	engine(void);
	virtual ~engine(void);

public:
	bool start(uv_loop_t *loop);
	void stop(void);
	void handle_event(const SDL_Event &event);

private:
	void frame(void);
	static void frame_cb(uv_timer_t *timer);
	void state(void);
	static void state_cb(uv_timer_t *timer);

private:
	uv_loop_t *m_loop;
	uv_timer_t m_frame_timer;
	uv_timer_t m_state_timer;
	unsigned int m_fps;
	unsigned int m_fps_count;
	uint64_t m_last_time;
	float m_elapsed;
	bool m_gui_mode;
};

SGE_END

#endif // SGE_ENGINE_HPP
