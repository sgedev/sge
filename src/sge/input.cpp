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
	float xoffset = event.xrel * m_mouse_sensitivity;
	float yoffset = event.yrel * m_mouse_sensitivity;

	if (m_mouse_handler)
		m_mouse_handler(xoffset, yoffset);

	return true;
}

bool input::init(void)
{
	m_keymap.clear();

	m_mouse_sensitivity = 1.0f;
	m_mouse_handler = NULL;

	return true;
}

void input::shutdown(void)
{
	m_keymap.clear();
}

void input::update(float elapsed)
{
	for (auto it = m_keymap.begin(); it != m_keymap.end(); ++it) {
		if (it->second.state != SDL_PRESSED)
			continue;
		if (it->second.handler)
			it->second.handler();
	}
}

void input::handle_event(const SDL_Event &event)
{
	switch (event.type) {
	case SDL_KEYDOWN:
	case SDL_KEYUP:
		handle_key_state(event.key);
		break;
	case SDL_MOUSEMOTION:
		handle_mouse_motion(event.motion);
		break;
	}
}

void input::bind(SDL_Keycode key, key_handler handler)
{
	SGE_ASSERT(handler != NULL);

	key_state &ks = m_keymap[key];

	ks.state = SDL_RELEASED;
	ks.handler = handler;
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

void input::set_mouse_handler(mouse_handler handler)
{
	m_mouse_handler = handler;
}

SGE_END

