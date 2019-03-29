//
//
#ifndef SGE_ENGINE_HPP
#define SGE_ENGINE_HPP

#include <uv.h>
#include <SDL.h>

#include <sge/common.hpp>
#include <sge/gl.hpp>
#include <sge/gui.hpp>
#include <sge/camera.hpp>
#include <sge/scene.hpp>

SGE_BEGIN

class engine {
public:
	engine(void);
	virtual ~engine(void);

public:
	bool start(uv_loop_t *loop = NULL);
	void stop(void);

protected:
	uv_loop_t *get_loop(void);
	gl::context &get_gl_context(void);

protected:
	virtual bool preinit(void);
	virtual bool init(void);
	virtual void shutdown(void);
	virtual void postshutdown(void);
	virtual void update(float elapsed);
	virtual void draw(void);
	virtual void handle_event(const SDL_Event *event);

private:
	void handle_window_event(const SDL_WindowEvent *event);
	void poll_events(void);
	void frame(void);
	void state(void);
	static void frame_cb(uv_timer_t *p);
	static void state_cb(uv_timer_t *p);

private:
	uv_timer_t m_frame_timer;
	uv_timer_t m_state_timer;
	uint64_t m_frame_last_time;
	int m_frame_count;
	int m_frame_count_per_second;
	bool m_started;
	bool m_drawable;
	gl::context m_gl_context;
	uv_loop_t *m_loop;
};

inline uv_loop_t *engine::get_loop(void)
{
	return m_loop;
}

inline gl::context &engine::get_gl_context(void)
{
	return m_gl_context;
}

SGE_END

#endif // SGE_ENGINE_HPP

