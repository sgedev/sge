//
//
#ifndef SGE_ENGINE_HPP
#define SGE_ENGINE_HPP

#include <sge/common.hpp>
#include <sge/noncopyable.hpp>
#include <sge/gl.hpp>
#include <sge/scene.hpp>
#include <sge/gui.hpp>

SGE_BEGIN

class engine: public noncopyable {
public:
	engine(uv_loop_t *loop = NULL);
	virtual ~engine(void);

public:
	bool start(void);
	void stop(void);
	void handle_event(const SDL_Event &event);
	unsigned int get_fps(void);
	SDL_Window *get_window(void);
	scene::system &get_scene(void);
	gui::system &get_gui(void);

protected:
	virtual bool init(void);
	virtual void shutdown(void);
	virtual void update(float elapsed);
	virtual void draw(void);

private:
	void frame(void);
	static void frame_cb(uv_timer_t *p);
	void state(void);
	static void state_cb(uv_timer_t *p);
	void handle_window_event(const SDL_WindowEvent &event);

private:
	uv_loop_t *m_loop;
	uv_timer_t m_frame_timer;
	uv_timer_t m_state_timer;
	uint64_t m_time_base;
	float m_time_scale;
	unsigned int m_fps_count;
	unsigned int m_fps;
	bool m_started;

private:
	SDL_Window *m_window;
	SDL_GLContext m_gl_context;
	Uint32 m_window_id;
	bool m_visibled;

private:
	scene::system m_scene;
	gui::system m_gui;
};

inline unsigned int engine::get_fps(void)
{
	return m_fps;
}

inline SDL_Window *engine::get_window(void)
{
	SGE_ASSERT(m_window != NULL);

	return m_window;
}

inline scene::system &engine::get_scene(void)
{
	return m_scene;
}

inline gui::system &engine::get_gui(void)
{
	return m_gui;
}

SGE_END

#endif // SGE_ENGINE_HPP

