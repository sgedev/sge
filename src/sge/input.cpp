//
//
#include <map>

#include <sge/input.hpp>

SGE_INPUT_BEGIN

struct key_state {
	int state;
	key_handler handler;
	void *data;
};

typedef std::map<SDL_Keycode, key_state> keymap;

static keymap s_keymap;
static float s_mouse_sensitivity;
static mouse_motion_handler s_mouse_motion_handler;
static void *s_mouse_motion_data;

static bool handle_key_state(const SDL_KeyboardEvent &event)
{
	keymap::iterator it = s_keymap.find(event.keysym.sym);
	if (it == s_keymap.end())
		return false;

	SGE_ASSERT(it->second.handler != NULL);

	it->second.state = event.state;

	return true;
}

static bool handle_mouse_motion(const SDL_MouseMotionEvent &event)
{
	if (s_mouse_motion_handler == NULL)
		return false;

	float xoffset = event.xrel * s_mouse_sensitivity;
	float yoffset = event.yrel * s_mouse_sensitivity;

	s_mouse_motion_handler(xoffset, yoffset, s_mouse_motion_data);

	return true;
}

bool init(void)
{
	s_keymap.clear();

	s_mouse_sensitivity = 1.0f;
	s_mouse_motion_handler = NULL;
	s_mouse_motion_data = NULL;

	return true;
}

void shutdown(void)
{
	s_keymap.clear();

	s_mouse_motion_handler = NULL;
}

void update(void)
{
	for (auto it = s_keymap.begin(); it != s_keymap.end(); ++it) {
		if (it->second.state != SDL_PRESSED)
			continue;
		SGE_ASSERT(it->second.handler != NULL);
		it->second.handler(it->first, it->second.data);
	}
}

bool handle_event(const SDL_Event &event)
{
	switch (event.type) {
	case SDL_KEYDOWN:
	case SDL_KEYUP:
		return handle_key_state(event.key);
	case SDL_MOUSEMOTION:
		return handle_mouse_motion(event.motion);
	}

	return false;
}

void bind(SDL_Keycode key, key_handler handler, void *data)
{
	SGE_ASSERT(handler != NULL);

	key_state &ks = s_keymap[key];

	ks.handler = handler;
	ks.data = data;
	ks.state = SDL_RELEASED;
}

void unbind(SDL_Keycode key)
{
	auto it = s_keymap.find(key);
	if (it != s_keymap.end())
		s_keymap.erase(it);
}

void unbindall(void)
{
	s_keymap.clear();
}

void set_mouse_sensitivity(float v)
{
	s_mouse_sensitivity = glm::clamp(v, 0.001f, 30.0f);
}

void set_mouse_motion_handler(mouse_motion_handler handler, void *data)
{
	s_mouse_motion_handler = handler;
	s_mouse_motion_data = data;
}

SGE_INPUT_END

