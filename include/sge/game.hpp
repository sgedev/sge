//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <sge/common.hpp>
#include <sge/window.hpp>
#include <sge/input.hpp>
#include <sge/scene.hpp>
#include <sge/camera.hpp>
#include <sge/renderer.hpp>

SGE_BEGIN

class game {
public:
	game(uv_loop_t *loop = NULL);
	virtual ~game(void);

public:
	bool init(window *pw);
	void shutdown(void);
	void handle_event(const SDL_Event &event);
	void update(float elapsed);
	void draw(void);
	void reset(void);
	void clear(void);
	bool load(const char *path);

private:
	uv_loop_t *m_loop;
	window *m_window;
	input m_input;
	scene m_scene;
	renderer m_renderer;
	camera m_camera;
};

SGE_END

#endif // SGE_GAME_HPP

