//
//
#include <sge/input.hpp>

SGE_BEGIN

input::input(void)
{
}

input::~input(void)
{
}

bool input::handle_key_state(const SDL_KeyboardEvent &event)
{
	keymap::iterator it = m_keymap.find(event.keysym.sym);
	if (it == m_keymap.end())
		return false;

	SGE_ASSERT(it->second.handler != NULL);

	it->second.state = event.state;

	return true;
}

bool input::handle_mouse_motion(const SDL_MouseMotionEvent &event)
{
	if (m_mouse_motion_handler == NULL)
		return false;

	float xoffset = event.xrel * m_mouse_sensitivity;
	float yoffset = event.yrel * m_mouse_sensitivity;

	m_mouse_motion_handler(xoffset, yoffset, m_mouse_motion_data);

	return true;
}

bool input::init(void)
{
	m_keymap.clear();

	m_mouse_sensitivity = 1.0f;
	m_mouse_motion_handler = NULL;
	m_mouse_motion_data = NULL;

	return true;
}

void input::shutdown(void)
{
	m_keymap.clear();

	m_mouse_motion_handler = NULL;
}

void input::update(float elapsed)
{
	for (auto it = m_keymap.begin(); it != m_keymap.end(); ++it) {
		if (it->second.state != SDL_PRESSED)
			continue;
		SGE_ASSERT(it->second.handler != NULL);
		it->second.handler(it->first, it->second.data);
	}
}

bool input::handle_event(const SDL_Event &event)
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

void input::bind(SDL_Keycode key, key_handler handler, void *data)
{
	SGE_ASSERT(handler != NULL);

	key_state &ks = m_keymap[key];

	ks.handler = handler;
	ks.data = data;
	ks.state = SDL_RELEASED;
}

void input::unbind(SDL_Keycode key)
{
	auto it = m_keymap.find(key);
	if (it != m_keymap.end())
		m_keymap.erase(it);
}

void input::unbindall(void)
{
	m_keymap.clear();
}

void input::set_mouse_sensitivity(float v)
{
	m_mouse_sensitivity = glm::clamp(v, 0.001f, 30.0f);
}

void input::set_mouse_motion_handler(mouse_motion_handler handler, void *data)
{
	m_mouse_motion_handler = handler;
	m_mouse_motion_data = data;
}

SGE_END

