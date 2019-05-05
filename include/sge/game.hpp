//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <type_traits>

#include <sge/common.hpp>
#include <sge/gl.hpp>
#include <sge/renderer.hpp>

SGE_BEGIN

class game {
public:
	game(uv_loop_t *loop);
	virtual ~game(void);

public:
	bool start(void);
	void stop(void);
	unsigned int fps(void) const;
	renderer::view &default_view(void);

protected:
	virtual bool init(void);
	virtual void shutdown(void);
	virtual void handle_event(const SDL_Event &event);
	virtual void update(float elapsed);
	virtual void draw(void);

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

private:
	gl::window m_window;
	gl::context m_gl_context;
	renderer::view m_view;
};

inline unsigned int game::fps(void) const
{
	return m_fps;
}

inline renderer::view &game::default_view(void)
{
	return m_view;
}

SGE_END

#define SGE_GAME(game_class) \
	::sge::game *sge_game_new(uv_loop_t *loop) { \
		static_assert(std::is_base_of<sge::game, game_class>::value); \
		uv_loop_t *lp = loop; \
		return new game_class(lp == NULL ? uv_default_loop() : lp); \
	}

#endif // SGE_GAME_HPP
