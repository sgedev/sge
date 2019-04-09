//
//
#ifndef SGE_ENGINE_HPP
#define SGE_ENGINE_HPP

#include <sge/common.hpp>
#include <sge/renderer.hpp>
#include <sge/gui.hpp>
#include <sge/scene.hpp>

SGE_BEGIN

class engine {
public:
	engine(uv_loop_t *loop = NULL);
	virtual ~engine(void);

public:
	bool start(void);
	void stop(void);
	void feed_event(const SDL_Event &event);

protected:
	float get_elapsed(void) const;
	renderer::context get_renderer(void);
	gui::context &get_gui(void);
	scene::context &get_scene(void);

protected:
	virtual bool preinit(void);
	virtual bool init(void);
	virtual void shutdown(void);
	virtual void postshutdown(void);
	virtual void update(float elapsed);
	virtual void draw(renderer::context &r);
	virtual void handle_event(const SDL_Event &event);

private:
	void frame(void);
	void state(void);
	static void frame_cb(uv_timer_t *p);
	static void state_cb(uv_timer_t *p);

private:
	uv_loop_t *m_loop;
	uv_timer_t m_frame_timer;
	uv_timer_t m_state_timer;
	renderer::context m_renderer;
	gui::context m_gui;
	scene::context m_scene;
	float m_elapsed;
	float m_time_scale;
};

inline float engine::get_elapsed(void) const
{
	return m_elapsed;
}

inline renderer::context engine::get_renderer(void)
{
	return m_renderer;
}

inline gui::context &engine::get_gui(void)
{
	return m_gui;
}

inline scene::context &engine::get_scene(void)
{
	return m_scene;
}

SGE_END

#endif // SGE_ENGINE_HPP

