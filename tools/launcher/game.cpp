//
//
#include <sge/keycode.hpp>
#include <sge/event.hpp>

#include "game.hpp"

static inline int MapKey(SDL_Keycode k)
{
	int ret;

	switch (k) {
	case SDLK_RETURN: ret = SGE::KEY_RETURN; break;
	case SDLK_ESCAPE: ret = SGE::KEY_ESCAPE; break;
	case SDLK_BACKSPACE: ret = SGE::KEY_BACKSPACE; break;
	case SDLK_TAB: ret = SGE::KEY_TAB; break;
	case SDLK_SPACE: ret = SGE::KEY_SPACE; break;
	case SDLK_EXCLAIM: ret = SGE::KEY_EXCLAIM; break;
	case SDLK_QUOTEDBL: ret = SGE::KEY_QUOTEDBL; break;
	case SDLK_HASH: ret = SGE::KEY_HASH; break;
	case SDLK_PERCENT: ret = SGE::KEY_PERCENT; break;
	case SDLK_DOLLAR: ret = SGE::KEY_DOLLAR; break;
	case SDLK_AMPERSAND: ret = SGE::KEY_AMPERSAND; break;
	case SDLK_QUOTE: ret = SGE::KEY_QUOTE; break;
	case SDLK_LEFTPAREN: ret = SGE::KEY_LEFTPAREN; break;
	case SDLK_RIGHTPAREN: ret = SGE::KEY_RIGHTPAREN; break;
	case SDLK_ASTERISK: ret = SGE::KEY_ASTERISK; break;
	case SDLK_PLUS: ret = SGE::KEY_PLUS; break;
	case SDLK_COMMA: ret = SGE::KEY_COMMA; break;
	case SDLK_MINUS: ret = SGE::KEY_MINUS; break;
	case SDLK_PERIOD: ret = SGE::KEY_PERIOD; break;
	case SDLK_SLASH: ret = SGE::KEY_SLASH; break;
	case SDLK_0: ret = SGE::KEY_0; break;
	case SDLK_1: ret = SGE::KEY_1; break;
	case SDLK_2: ret = SGE::KEY_2; break;
	case SDLK_3: ret = SGE::KEY_3; break;
	case SDLK_4: ret = SGE::KEY_4; break;
	case SDLK_5: ret = SGE::KEY_5; break;
	case SDLK_6: ret = SGE::KEY_6; break;
	case SDLK_7: ret = SGE::KEY_7; break;
	case SDLK_8: ret = SGE::KEY_8; break;
	case SDLK_9: ret = SGE::KEY_9; break;
	case SDLK_COLON: ret = SGE::KEY_COLON; break;
	case SDLK_SEMICOLON: ret = SGE::KEY_SEMICOLON; break;
	case SDLK_LESS: ret = SGE::KEY_LESS; break;
	case SDLK_EQUALS: ret = SGE::KEY_EQUALS; break;
	case SDLK_GREATER: ret = SGE::KEY_GREATER; break;
	case SDLK_QUESTION: ret = SGE::KEY_QUESTION; break;
	case SDLK_AT: ret = SGE::KEY_AT; break;
	case SDLK_LEFTBRACKET: ret = SGE::KEY_LEFTBRACKET; break;
	case SDLK_BACKSLASH: ret = SGE::KEY_BACKSLASH; break;
	case SDLK_RIGHTBRACKET: ret = SGE::KEY_RIGHTBRACKET; break;
	case SDLK_CARET: ret = SGE::KEY_CARET; break;
	case SDLK_UNDERSCORE: ret = SGE::KEY_UNDERSCORE; break;
	case SDLK_BACKQUOTE: ret = SGE::KEY_BACKQUOTE; break;
	case SDLK_a: ret = SGE::KEY_A; break;
	case SDLK_b: ret = SGE::KEY_B; break;
	case SDLK_c: ret = SGE::KEY_C; break;
	case SDLK_d: ret = SGE::KEY_D; break;
	case SDLK_e: ret = SGE::KEY_E; break;
	case SDLK_f: ret = SGE::KEY_F; break;
	case SDLK_g: ret = SGE::KEY_G; break;
	case SDLK_h: ret = SGE::KEY_H; break;
	case SDLK_i: ret = SGE::KEY_I; break;
	case SDLK_j: ret = SGE::KEY_J; break;
	case SDLK_k: ret = SGE::KEY_K; break;
	case SDLK_l: ret = SGE::KEY_L; break;
	case SDLK_m: ret = SGE::KEY_M; break;
	case SDLK_n: ret = SGE::KEY_N; break;
	case SDLK_o: ret = SGE::KEY_O; break;
	case SDLK_p: ret = SGE::KEY_P; break;
	case SDLK_q: ret = SGE::KEY_Q; break;
	case SDLK_r: ret = SGE::KEY_R; break;
	case SDLK_s: ret = SGE::KEY_S; break;
	case SDLK_t: ret = SGE::KEY_T; break;
	case SDLK_u: ret = SGE::KEY_U; break;
	case SDLK_v: ret = SGE::KEY_V; break;
	case SDLK_w: ret = SGE::KEY_W; break;
	case SDLK_x: ret = SGE::KEY_X; break;
	case SDLK_y: ret = SGE::KEY_Y; break;
	case SDLK_z: ret = SGE::KEY_Z; break;
	case SDLK_CAPSLOCK: ret = SGE::KEY_CAPSLOCK; break;
	case SDLK_F1: ret = SGE::KEY_F1; break;
	case SDLK_F2: ret = SGE::KEY_F2; break;
	case SDLK_F3: ret = SGE::KEY_F3; break;
	case SDLK_F4: ret = SGE::KEY_F4; break;
	case SDLK_F5: ret = SGE::KEY_F5; break;
	case SDLK_F6: ret = SGE::KEY_F6; break;
	case SDLK_F7: ret = SGE::KEY_F7; break;
	case SDLK_F8: ret = SGE::KEY_F8; break;
	case SDLK_F9: ret = SGE::KEY_F9; break;
	case SDLK_F10: ret = SGE::KEY_F10; break;
	case SDLK_F11: ret = SGE::KEY_F11; break;
	case SDLK_F12: ret = SGE::KEY_F12; break;
	case SDLK_PRINTSCREEN: ret = SGE::KEY_PRINTSCREEN; break;
	case SDLK_SCROLLLOCK: ret = SGE::KEY_SCROLLLOCK; break;
	case SDLK_PAUSE: ret = SGE::KEY_PAUSE; break;
	case SDLK_INSERT: ret = SGE::KEY_INSERT; break;
	case SDLK_HOME: ret = SGE::KEY_HOME; break;
	case SDLK_PAGEUP: ret = SGE::KEY_PAGEUP; break;
	case SDLK_DELETE: ret = SGE::KEY_DELETE; break;
	case SDLK_END: ret = SGE::KEY_END; break;
	case SDLK_PAGEDOWN: ret = SGE::KEY_PAGEDOWN; break;
	case SDLK_RIGHT: ret = SGE::KEY_RIGHT; break;
	case SDLK_LEFT: ret = SGE::KEY_LEFT; break;
	case SDLK_DOWN: ret = SGE::KEY_DOWN; break;
	case SDLK_UP: ret = SGE::KEY_UP; break;
	case SDLK_NUMLOCKCLEAR: ret = SGE::KEY_NUMLOCKCLEAR; break;
	case SDLK_KP_DIVIDE: ret = SGE::KEY_KP_DIVIDE; break;
	case SDLK_KP_MULTIPLY: ret = SGE::KEY_KP_MULTIPLY; break;
	case SDLK_KP_MINUS: ret = SGE::KEY_KP_MINUS; break;
	case SDLK_KP_PLUS: ret = SGE::KEY_KP_PLUS; break;
	case SDLK_KP_ENTER: ret = SGE::KEY_KP_ENTER; break;
	case SDLK_KP_1: ret = SGE::KEY_KP_1; break;
	case SDLK_KP_2: ret = SGE::KEY_KP_2; break;
	case SDLK_KP_3: ret = SGE::KEY_KP_3; break;
	case SDLK_KP_4: ret = SGE::KEY_KP_4; break;
	case SDLK_KP_5: ret = SGE::KEY_KP_5; break;
	case SDLK_KP_6: ret = SGE::KEY_KP_6; break;
	case SDLK_KP_7: ret = SGE::KEY_KP_7; break;
	case SDLK_KP_8: ret = SGE::KEY_KP_8; break;
	case SDLK_KP_9: ret = SGE::KEY_KP_9; break;
	case SDLK_KP_0: ret = SGE::KEY_KP_0; break;
	case SDLK_KP_PERIOD: ret = SGE::KEY_KP_PERIOD; break;
	case SDLK_APPLICATION: ret = SGE::KEY_APPLICATION; break;
	case SDLK_POWER: ret = SGE::KEY_POWER; break;
	case SDLK_KP_EQUALS: ret = SGE::KEY_KP_EQUALS; break;
	case SDLK_F13: ret = SGE::KEY_F13; break;
	case SDLK_F14: ret = SGE::KEY_F14; break;
	case SDLK_F15: ret = SGE::KEY_F15; break;
	case SDLK_F16: ret = SGE::KEY_F16; break;
	case SDLK_F17: ret = SGE::KEY_F17; break;
	case SDLK_F18: ret = SGE::KEY_F18; break;
	case SDLK_F19: ret = SGE::KEY_F19; break;
	case SDLK_F20: ret = SGE::KEY_F20; break;
	case SDLK_F21: ret = SGE::KEY_F21; break;
	case SDLK_F22: ret = SGE::KEY_F22; break;
	case SDLK_F23: ret = SGE::KEY_F23; break;
	case SDLK_F24: ret = SGE::KEY_F24; break;
	case SDLK_EXECUTE: ret = SGE::KEY_EXEC; break;
	case SDLK_HELP: ret = SGE::KEY_HELP; break;
	case SDLK_MENU: ret = SGE::KEY_MENU; break;
	case SDLK_SELECT: ret = SGE::KEY_SELECT; break;
	case SDLK_STOP: ret = SGE::KEY_STOP; break;
	case SDLK_AGAIN: ret = SGE::KEY_AGAIN; break;
	case SDLK_UNDO: ret = SGE::KEY_UNDO; break;
	case SDLK_CUT: ret = SGE::KEY_CUT; break;
	case SDLK_COPY: ret = SGE::KEY_COPY; break;
	case SDLK_PASTE: ret = SGE::KEY_PASTE; break;
	case SDLK_FIND: ret = SGE::KEY_FIND; break;
	case SDLK_MUTE: ret = SGE::KEY_MUTE; break;
	case SDLK_VOLUMEUP: ret = SGE::KEY_VOLUMEUP; break;
	case SDLK_VOLUMEDOWN: ret = SGE::KEY_VOLUMEDOWN; break;
	case SDLK_KP_COMMA: ret = SGE::KEY_KP_COMMA; break;
	case SDLK_KP_EQUALSAS400: ret = SGE::KEY_KP_EQUALSAS400; break;
	case SDLK_ALTERASE: ret = SGE::KEY_ALTERASE; break;
	case SDLK_SYSREQ: ret = SGE::KEY_SYSREQ; break;
	case SDLK_CANCEL: ret = SGE::KEY_CANCEL; break;
	case SDLK_CLEAR: ret = SGE::KEY_CLEAR; break;
	case SDLK_PRIOR: ret = SGE::KEY_PRIOR; break;
	case SDLK_RETURN2: ret = SGE::KEY_RETURN2; break;
	case SDLK_SEPARATOR: ret = SGE::KEY_SEPARATOR; break;
	case SDLK_OUT: ret = SGE::KEY_OUT; break;
	case SDLK_OPER: ret = SGE::KEY_OPER; break;
	case SDLK_CLEARAGAIN: ret = SGE::KEY_CLEARAGAIN; break;
	case SDLK_CRSEL: ret = SGE::KEY_CRSEL; break;
	case SDLK_EXSEL: ret = SGE::KEY_EXSEL; break;
	case SDLK_THOUSANDSSEPARATOR: ret = SGE::KEY_THOUSANDSSEPARATOR; break;
	case SDLK_DECIMALSEPARATOR: ret = SGE::KEY_DECIMALSEPARATOR; break;
	case SDLK_CURRENCYUNIT: ret = SGE::KEY_CURRENCYUNIT; break;
	case SDLK_CURRENCYSUBUNIT: ret = SGE::KEY_CURRENCYSUBUNIT; break;
	case SDLK_KP_LEFTPAREN: ret = SGE::KEY_KP_LEFTPAREN; break;
	case SDLK_KP_RIGHTPAREN: ret = SGE::KEY_KP_RIGHTPAREN; break;
	case SDLK_KP_LEFTBRACE: ret = SGE::KEY_KP_LEFTBRACE; break;
	case SDLK_KP_RIGHTBRACE: ret = SGE::KEY_KP_RIGHTBRACE; break;
	case SDLK_KP_TAB: ret = SGE::KEY_KP_TAB; break;
	case SDLK_KP_BACKSPACE: ret = SGE::KEY_KP_BACKSPACE; break;
	case SDLK_KP_A: ret = SGE::KEY_KP_A; break;
	case SDLK_KP_B: ret = SGE::KEY_KP_B; break;
	case SDLK_KP_C: ret = SGE::KEY_KP_C; break;
	case SDLK_KP_D: ret = SGE::KEY_KP_D; break;
	case SDLK_KP_E: ret = SGE::KEY_KP_E; break;
	case SDLK_KP_F: ret = SGE::KEY_KP_F; break;
	case SDLK_KP_XOR: ret = SGE::KEY_KP_XOR; break;
	case SDLK_KP_POWER: ret = SGE::KEY_KP_POWER; break;
	case SDLK_KP_PERCENT: ret = SGE::KEY_KP_PERCENT; break;
	case SDLK_KP_LESS: ret = SGE::KEY_KP_LESS; break;
	case SDLK_KP_GREATER: ret = SGE::KEY_KP_GREATER; break;
	case SDLK_KP_AMPERSAND: ret = SGE::KEY_KP_AMPERSAND; break;
	case SDLK_KP_DBLAMPERSAND: ret = SGE::KEY_KP_DBLAMPERSAND; break;
	case SDLK_KP_VERTICALBAR: ret = SGE::KEY_KP_VERTICALBAR; break;
	case SDLK_KP_DBLVERTICALBAR: ret = SGE::KEY_KP_DBLVERTICALBAR; break;
	case SDLK_KP_COLON: ret = SGE::KEY_KP_COLON; break;
	case SDLK_KP_HASH: ret = SGE::KEY_KP_HASH; break;
	case SDLK_KP_SPACE: ret = SGE::KEY_KP_SPACE; break;
	case SDLK_KP_AT: ret = SGE::KEY_KP_AT; break;
	case SDLK_KP_EXCLAM: ret = SGE::KEY_KP_EXCLAM; break;
	case SDLK_KP_MEMSTORE: ret = SGE::KEY_KP_MEMSTORE; break;
	case SDLK_KP_MEMRECALL: ret = SGE::KEY_KP_MEMRECALL; break;
	case SDLK_KP_MEMCLEAR: ret = SGE::KEY_KP_MEMCLEAR; break;
	case SDLK_KP_MEMADD: ret = SGE::KEY_KP_MEMADD; break;
	case SDLK_KP_MEMSUBTRACT: ret = SGE::KEY_KP_MEMSUBTRACT; break;
	case SDLK_KP_MEMMULTIPLY: ret = SGE::KEY_KP_MEMMULTIPLY; break;
	case SDLK_KP_MEMDIVIDE: ret = SGE::KEY_KP_MEMDIVIDE; break;
	case SDLK_KP_PLUSMINUS: ret = SGE::KEY_KP_PLUSMINUS; break;
	case SDLK_KP_CLEAR: ret = SGE::KEY_KP_CLEAR; break;
	case SDLK_KP_CLEARENTRY: ret = SGE::KEY_KP_CLEARENTRY; break;
	case SDLK_KP_BINARY: ret = SGE::KEY_KP_BINARY; break;
	case SDLK_KP_OCTAL: ret = SGE::KEY_KP_OCTAL; break;
	case SDLK_KP_DECIMAL: ret = SGE::KEY_KP_DECIMAL; break;
	case SDLK_KP_HEXADECIMAL: ret = SGE::KEY_KP_HEXADECIMAL; break;
	case SDLK_LCTRL: ret = SGE::KEY_LCTRL; break;
	case SDLK_LSHIFT: ret = SGE::KEY_LSHIFT; break;
	case SDLK_LALT: ret = SGE::KEY_LALT; break;
	case SDLK_LGUI: ret = SGE::KEY_LGUI; break;
	case SDLK_RCTRL: ret = SGE::KEY_RCTRL; break;
	case SDLK_RSHIFT: ret = SGE::KEY_RSHIFT; break;
	case SDLK_RALT: ret = SGE::KEY_RALT; break;
	case SDLK_RGUI: ret = SGE::KEY_RGUI; break;
	case SDLK_MODE: ret = SGE::KEY_MODE; break;
	case SDLK_AUDIONEXT: ret = SGE::KEY_AUDIONEXT; break;
	case SDLK_AUDIOPREV: ret = SGE::KEY_AUDIOPREV; break;
	case SDLK_AUDIOSTOP: ret = SGE::KEY_AUDIOSTOP; break;
	case SDLK_AUDIOPLAY: ret = SGE::KEY_AUDIOPLAY; break;
	case SDLK_AUDIOMUTE: ret = SGE::KEY_AUDIOMUTE; break;
	case SDLK_MEDIASELECT: ret = SGE::KEY_MEDIASELECT; break;
	case SDLK_WWW: ret = SGE::KEY_WWW; break;
	case SDLK_MAIL: ret = SGE::KEY_MAIL; break;
	case SDLK_CALCULATOR: ret = SGE::KEY_CALCULATOR; break;
	case SDLK_COMPUTER: ret = SGE::KEY_COMPUTER; break;
	case SDLK_AC_SEARCH: ret = SGE::KEY_AC_SEARCH; break;
	case SDLK_AC_HOME: ret = SGE::KEY_AC_HOME; break;
	case SDLK_AC_BACK: ret = SGE::KEY_AC_BACK; break;
	case SDLK_AC_FORWARD: ret = SGE::KEY_AC_FORWARD; break;
	case SDLK_AC_STOP: ret = SGE::KEY_AC_STOP; break;
	case SDLK_AC_REFRESH: ret = SGE::KEY_AC_REFRESH; break;
	case SDLK_AC_BOOKMARKS: ret = SGE::KEY_AC_BOOKMARKS; break;
	case SDLK_BRIGHTNESSDOWN: ret = SGE::KEY_BRIGHTNESSDOWN; break;
	case SDLK_BRIGHTNESSUP: ret = SGE::KEY_BRIGHTNESSUP; break;
	case SDLK_DISPLAYSWITCH: ret = SGE::KEY_DISPLAYSWITCH; break;
	case SDLK_KBDILLUMTOGGLE: ret = SGE::KEY_KBDILLUMTOGGLE; break;
	case SDLK_KBDILLUMDOWN: ret = SGE::KEY_KBDILLUMDOWN; break;
	case SDLK_KBDILLUMUP: ret = SGE::KEY_KBDILLUMUP; break;
	case SDLK_EJECT: ret = SGE::KEY_EJECT; break;
	case SDLK_SLEEP: ret = SGE::KEY_SLEEP; break;
	default: ret = SGE::KEY_UNKNOWN; break;
	}

	return ret;
}

Game::Game(void)
	: m_window(NULL)
	, m_windowVisibled(false)
	, m_gl(NULL)
{
}

Game::~Game(void)
{
	if (m_window != NULL)
		shutdown();
}

bool Game::init(const char *root)
{
	SGE_ASSERT(m_window == NULL);

	if (!SGE::Game::init(root))
		goto bad0;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

#ifdef SGE_DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	m_window = SDL_CreateWindow("SGE",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

	if (m_window == NULL)
		goto bad1;

	m_windowId = SDL_GetWindowID(m_window);
	SDL_GetWindowPosition(m_window, &m_windowRect[0], &m_windowRect[1]);
	SDL_GetWindowSize(m_window, &m_windowRect[2], &m_windowRect[3]);

	m_gl = SDL_GL_CreateContext(m_window);
	if (m_gl == NULL)
		goto bad2;

	SDL_GL_MakeCurrent(m_window, m_gl);

	if (gl3wInit(&m_gl3w, (GL3WGetProcAddressProc)SDL_GL_GetProcAddress) != GL3W_OK)
		goto bad3;

	gl3wProcs = &m_gl3w;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	if (!m_view.init())
		goto bad3;

	return true;

bad3:
	SDL_GL_DeleteContext(m_gl);
	m_gl = NULL;

bad2:
	SDL_DestroyWindow(m_window);
	m_window = NULL;

bad1:
	SGE::Game::shutdown();

bad0:
	return false;
}

void Game::shutdown(void)
{
	SGE_ASSERT(m_window != NULL);

	m_view.shutdown();

	SDL_GL_DeleteContext(m_gl);
	m_gl = NULL;

	SDL_DestroyWindow(m_window);
	m_window = NULL;

	SGE::Game::shutdown();
}

bool Game::handleEvent(const SDL_Event *event)
{
	if (event == NULL)
		return false;

	bool ret;

	switch (event->type) {
	case SDL_KEYDOWN:
	case SDL_KEYUP:
		ret = handleKeyEvent(event);
		break;
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		ret = handleMouseButtonEvent(event);
		break;
	case SDL_MOUSEMOTION:
		ret = handleMouseMoveEvent(event);
		break;
	case SDL_WINDOWEVENT:
		ret = handleWindowEvent(event);
		break;
	default:
		ret = false;
		break;
	}

	return ret;
}

void Game::frame(float elapsed)
{
	update(elapsed);

	if (m_windowVisibled && m_windowRect[2] > 1 && m_windowRect[3] > 1) {
		gl3wProcs = &m_gl3w;
		glViewport(0, 0, m_windowRect[2], m_windowRect[3]);
		glClear(GL_COLOR_BUFFER_BIT);
		m_view.beginFrame();
		draw(&m_view);
		m_view.endFrame();
		SDL_GL_SwapWindow(m_window);
	}
}

int Game::trapFps(lua_State *L)
{
	return TRAP_RESULT_NOT_SUPPORT;
}

int Game::trapEditorIsEnabled(lua_State *L)
{
	lua_pushboolean(L, 0);
	return 1;
}

bool Game::handleKeyEvent(const SDL_Event *event)
{
	SGE::Event evt;
	
	switch (event->type) {
	case SDL_KEYDOWN:
		evt.type = SGE::Event::TYPE_KEY_DOWN;
		break;
	case SDL_KEYUP:
		evt.type = SGE::Event::TYPE_KEY_UP;
		break;
	default:
		return false;
	}

	evt.value.v_key.keycode = MapKey(event->key.keysym.sym);
	if (evt.value.v_key.keycode == SGE::KEY_UNKNOWN)
		return false;

	return SGE::Game::handleEvent(&evt);
}

bool Game::handleMouseButtonEvent(const SDL_Event *event)
{
	SGE::Event evt;
	
	switch (event->type) {
	case SDL_KEYDOWN:
		evt.type = SGE::Event::TYPE_MOUSE_BUTTON_DOWN;
		break;
	case SDL_KEYUP:
		evt.type = SGE::Event::TYPE_MOUSE_BUTTON_UP;
		break;
	default:
		SGE_ASSERT(false);
		break;
	}

	switch (event->button.button) {
	case SDL_BUTTON_LEFT:
		evt.value.v_mouseButton.button = SGE::Event::MouseButton::BUTTON1;
		break;
	case SDL_BUTTON_MIDDLE:
		evt.value.v_mouseButton.button = SGE::Event::MouseButton::BUTTON3;
		break;
	case SDL_BUTTON_RIGHT:
		evt.value.v_mouseButton.button = SGE::Event::MouseButton::BUTTON2;
		break;
	default:
		return false;
	}

	return SGE::Game::handleEvent(&evt);
}

bool Game::handleMouseMoveEvent(const SDL_Event *event)
{
	SGE_ASSERT(event->type == SDL_MOUSEMOTION);

	SGE::Event evt(SGE::Event::TYPE_MOUSE_MOVE);

	evt.value.v_mouseMove.dx = event->motion.xrel;
	evt.value.v_mouseMove.dy = event->motion.yrel;

	return SGE::Game::handleEvent(&evt);
}

bool Game::handleWindowEvent(const SDL_Event *event)
{
	SGE_ASSERT(event->type == SDL_WINDOWEVENT);

	switch (event->window.event) {
	case SDL_WINDOWEVENT_MOVED:
		m_windowRect[0] = event->window.data1;
		m_windowRect[1] = event->window.data2;
		break;
	case SDL_WINDOWEVENT_RESIZED:
		m_windowRect[2] = event->window.data1;
		m_windowRect[3] = event->window.data2;
		break;
	case SDL_WINDOWEVENT_SHOWN:
		m_windowVisibled = true;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
	case SDL_WINDOWEVENT_MINIMIZED:
		m_windowVisibled = false;
		break;
	default:
		return false;
	}

	return true;
}
