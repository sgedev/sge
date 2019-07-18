//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <sge/common.hpp>
#include <sge/window.hpp>
#include <sge/input.hpp>
#include <sge/scene.hpp>
#include <sge/camera.hpp>

SGE_BEGIN

class game {
public:
	game(uv_loop_t *loop = NULL);
	virtual ~game(void);

public:
	bool init(void);
	void shutdown(void);
	void handle_event(const SDL_Event &event);
	void update(float elapsed);
	camera &get_camera(void);
	void reset(void);
	void clear(void);
	bool load(const char *path);

private:
	void move_forward(void);
	void move_backward(void);
	void move_left(void);
	void move_right(void);
	void mouse_view(int dx, int dy);

private:
	uv_loop_t *m_loop;
	input m_input;
	scene m_scene;
	camera m_camera;
};

inline camera &game::get_camera(void)
{
	return m_camera;
}

SGE_END

#endif // SGE_GAME_HPP

