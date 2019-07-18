//
//
#ifndef SGE_INPUT_HPP
#define SGE_INPUT_HPP

#include <map>

#include <sge/common.hpp>

SGE_BEGIN

class input {
public:
	typedef void (* key_handler)(SDL_Keycode key, void *data);
	typedef void (* mouse_motion_handler)(float xoffset, float yoffset, void *data);

public:
	input(void);
	virtual ~input(void);

public:
	bool init(void);
	void shutdown(void);
	void update(float elapsed);
	bool handle_event(const SDL_Event &event);
	void bind(SDL_Keycode key, key_handler handler, void *data);
	void unbind(SDL_Keycode key);
	void unbindall(void);
	void set_mouse_sensitivity(float v);
	void set_mouse_motion_handler(mouse_motion_handler handler, void *data);

private:
	bool handle_key_state(const SDL_KeyboardEvent &event);
	bool handle_mouse_motion(const SDL_MouseMotionEvent &event);

private:
	struct key_state {
		int state;
		key_handler handler;
		void *data;
	};

	typedef std::map<SDL_Keycode, key_state> keymap;

	keymap m_keymap;
	float m_mouse_sensitivity;
	mouse_motion_handler m_mouse_motion_handler;
	void *m_mouse_motion_data;
};

SGE_END

#endif // SGE_INPUT_HPP

