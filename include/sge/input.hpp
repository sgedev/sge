//
//
#ifndef SGE_INPUT_HPP
#define SGE_INPUT_HPP

#include <map>
#include <functional>

#include <sge/common.hpp>

SGE_BEGIN

class input {
public:
	typedef std::function<void(void)> key_handler;
	typedef std::function<void(float, float)> mouse_handler;

public:
	input(void);
	virtual ~input(void);

public:
	bool init(void);
	void shutdown(void);
	void update(float elapsed);
	void handle_event(const SDL_Event &event);
	void bind(SDL_Keycode key, key_handler handler);
	void unbind(SDL_Keycode key);
	void unbindall(void);
	void set_mouse_sensitivity(float v);
	void set_mouse_handler(mouse_handler handler);

private:
	bool handle_key_state(const SDL_KeyboardEvent &event);
	bool handle_mouse_motion(const SDL_MouseMotionEvent &event);

private:
	struct key_state {
		int state;
		key_handler handler;
	};

	typedef std::map<SDL_Keycode, key_state> keymap;

	keymap m_keymap;
	float m_mouse_sensitivity;
	mouse_handler m_mouse_handler;
};

SGE_END

#endif // SGE_INPUT_HPP

