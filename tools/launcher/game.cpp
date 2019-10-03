//
//
#include <sge/keycode.hpp>
#include <sge/event.hpp>

#include "game.hpp"

static inline int MapKey(SDL_Keycode k)
{
	int ret = sge::KEY_UNKNOWN;

	switch (k) {
	case SDLK_RETURN: ret = sge::KEY_RETURN; break;
	case SDLK_ESCAPE: ret = sge::KEY_ESCAPE; break;
	case SDLK_BACKSPACE: ret = sge::KEY_BACKSPACE; break;
	case SDLK_TAB: ret = sge::KEY_TAB; break;
	case SDLK_SPACE: ret = sge::KEY_SPACE; break;
	case SDLK_EXCLAIM: ret = sge::KEY_EXCLAIM; break;
	case SDLK_QUOTEDBL: ret = sge::KEY_QUOTEDBL; break;
	case SDLK_HASH: ret = sge::KEY_HASH; break;
	case SDLK_PERCENT: ret = sge::KEY_PERCENT; break;
	case SDLK_DOLLAR: ret = sge::KEY_DOLLAR; break;
	case SDLK_AMPERSAND: ret = sge::KEY_AMPERSAND; break;
	case SDLK_QUOTE: ret = sge::KEY_QUOTE; break;
	case SDLK_LEFTPAREN: ret = sge::KEY_LEFTPAREN; break;
	case SDLK_RIGHTPAREN: ret = sge::KEY_RIGHTPAREN; break;
	case SDLK_ASTERISK: ret = sge::KEY_ASTERISK; break;
	case SDLK_PLUS: ret = sge::KEY_PLUS; break;
	case SDLK_COMMA: ret = sge::KEY_COMMA; break;
	case SDLK_MINUS: ret = sge::KEY_MINUS; break;
	case SDLK_PERIOD: ret = sge::KEY_PERIOD; break;
	case SDLK_SLASH: ret = sge::KEY_SLASH; break;
	case SDLK_0: ret = sge::KEY_0; break;
	case SDLK_1: ret = sge::KEY_1; break;
	case SDLK_2: ret = sge::KEY_2; break;
	case SDLK_3: ret = sge::KEY_3; break;
	case SDLK_4: ret = sge::KEY_4; break;
	case SDLK_5: ret = sge::KEY_5; break;
	case SDLK_6: ret = sge::KEY_6; break;
	case SDLK_7: ret = sge::KEY_7; break;
	case SDLK_8: ret = sge::KEY_8; break;
	case SDLK_9: ret = sge::KEY_9; break;
	case SDLK_COLON: ret = sge::KEY_COLON; break;
	case SDLK_SEMICOLON: ret = sge::KEY_SEMICOLON; break;
	case SDLK_LESS: ret = sge::KEY_LESS; break;
	case SDLK_EQUALS: ret = sge::KEY_EQUALS; break;
	case SDLK_GREATER: ret = sge::KEY_GREATER; break;
	case SDLK_QUESTION: ret = sge::KEY_QUESTION; break;
	case SDLK_AT: ret = sge::KEY_AT; break;
	case SDLK_LEFTBRACKET: ret = sge::KEY_LEFTBRACKET; break;
	case SDLK_BACKSLASH: ret = sge::KEY_BACKSLASH; break;
	case SDLK_RIGHTBRACKET: ret = sge::KEY_RIGHTBRACKET; break;
	case SDLK_CARET: ret = sge::KEY_CARET; break;
	case SDLK_UNDERSCORE: ret = sge::KEY_UNDERSCORE; break;
	case SDLK_BACKQUOTE: ret = sge::KEY_BACKQUOTE; break;
	case SDLK_a: ret = sge::KEY_A; break;
	case SDLK_b: ret = sge::KEY_B; break;
	case SDLK_c: ret = sge::KEY_C; break;
	case SDLK_d: ret = sge::KEY_D; break;
	case SDLK_e: ret = sge::KEY_E; break;
	case SDLK_f: ret = sge::KEY_F; break;
	case SDLK_g: ret = sge::KEY_G; break;
	case SDLK_h: ret = sge::KEY_H; break;
	case SDLK_i: ret = sge::KEY_I; break;
	case SDLK_j: ret = sge::KEY_J; break;
	case SDLK_k: ret = sge::KEY_K; break;
	case SDLK_l: ret = sge::KEY_L; break;
	case SDLK_m: ret = sge::KEY_M; break;
	case SDLK_n: ret = sge::KEY_N; break;
	case SDLK_o: ret = sge::KEY_O; break;
	case SDLK_p: ret = sge::KEY_P; break;
	case SDLK_q: ret = sge::KEY_Q; break;
	case SDLK_r: ret = sge::KEY_R; break;
	case SDLK_s: ret = sge::KEY_S; break;
	case SDLK_t: ret = sge::KEY_T; break;
	case SDLK_u: ret = sge::KEY_U; break;
	case SDLK_v: ret = sge::KEY_V; break;
	case SDLK_w: ret = sge::KEY_W; break;
	case SDLK_x: ret = sge::KEY_X; break;
	case SDLK_y: ret = sge::KEY_Y; break;
	case SDLK_z: ret = sge::KEY_Z; break;
	case SDLK_CAPSLOCK: ret = sge::KEY_CAPSLOCK; break;
	case SDLK_F1: ret = sge::KEY_F1; break;
	case SDLK_F2: ret = sge::KEY_F2; break;
	case SDLK_F3: ret = sge::KEY_F3; break;
	case SDLK_F4: ret = sge::KEY_F4; break;
	case SDLK_F5: ret = sge::KEY_F5; break;
	case SDLK_F6: ret = sge::KEY_F6; break;
	case SDLK_F7: ret = sge::KEY_F7; break;
	case SDLK_F8: ret = sge::KEY_F8; break;
	case SDLK_F9: ret = sge::KEY_F9; break;
	case SDLK_F10: ret = sge::KEY_F10; break;
	case SDLK_F11: ret = sge::KEY_F11; break;
	case SDLK_F12: ret = sge::KEY_F12; break;
	case SDLK_PRINTSCREEN: ret = sge::KEY_PRINTSCREEN; break;
	case SDLK_SCROLLLOCK: ret = sge::KEY_SCROLLLOCK; break;
	case SDLK_PAUSE: ret = sge::KEY_PAUSE; break;
	case SDLK_INSERT: ret = sge::KEY_INSERT; break;
	case SDLK_HOME: ret = sge::KEY_HOME; break;
	case SDLK_PAGEUP: ret = sge::KEY_PAGEUP; break;
	case SDLK_DELETE: ret = sge::KEY_DELETE; break;
	case SDLK_END: ret = sge::KEY_END; break;
	case SDLK_PAGEDOWN: ret = sge::KEY_PAGEDOWN; break;
	case SDLK_RIGHT: ret = sge::KEY_RIGHT; break;
	case SDLK_LEFT: ret = sge::KEY_LEFT; break;
	case SDLK_DOWN: ret = sge::KEY_DOWN; break;
	case SDLK_UP: ret = sge::KEY_UP; break;
	case SDLK_NUMLOCKCLEAR: ret = sge::KEY_NUMLOCKCLEAR; break;
	case SDLK_KP_DIVIDE: ret = sge::KEY_KP_DIVIDE; break;
	case SDLK_KP_MULTIPLY: ret = sge::KEY_KP_MULTIPLY; break;
	case SDLK_KP_MINUS: ret = sge::KEY_KP_MINUS; break;
	case SDLK_KP_PLUS: ret = sge::KEY_KP_PLUS; break;
	case SDLK_KP_ENTER: ret = sge::KEY_KP_ENTER; break;
	case SDLK_KP_1: ret = sge::KEY_KP_1; break;
	case SDLK_KP_2: ret = sge::KEY_KP_2; break;
	case SDLK_KP_3: ret = sge::KEY_KP_3; break;
	case SDLK_KP_4: ret = sge::KEY_KP_4; break;
	case SDLK_KP_5: ret = sge::KEY_KP_5; break;
	case SDLK_KP_6: ret = sge::KEY_KP_6; break;
	case SDLK_KP_7: ret = sge::KEY_KP_7; break;
	case SDLK_KP_8: ret = sge::KEY_KP_8; break;
	case SDLK_KP_9: ret = sge::KEY_KP_9; break;
	case SDLK_KP_0: ret = sge::KEY_KP_0; break;
	case SDLK_KP_PERIOD: ret = sge::KEY_KP_PERIOD; break;
	case SDLK_APPLICATION: ret = sge::KEY_APPLICATION; break;
	case SDLK_POWER: ret = sge::KEY_POWER; break;
	case SDLK_KP_EQUALS: ret = sge::KEY_KP_EQUALS; break;
	case SDLK_F13: ret = sge::KEY_F13; break;
	case SDLK_F14: ret = sge::KEY_F14; break;
	case SDLK_F15: ret = sge::KEY_F15; break;
	case SDLK_F16: ret = sge::KEY_F16; break;
	case SDLK_F17: ret = sge::KEY_F17; break;
	case SDLK_F18: ret = sge::KEY_F18; break;
	case SDLK_F19: ret = sge::KEY_F19; break;
	case SDLK_F20: ret = sge::KEY_F20; break;
	case SDLK_F21: ret = sge::KEY_F21; break;
	case SDLK_F22: ret = sge::KEY_F22; break;
	case SDLK_F23: ret = sge::KEY_F23; break;
	case SDLK_F24: ret = sge::KEY_F24; break;
	case SDLK_EXECUTE: ret = sge::KEY_EXEC; break;
	case SDLK_HELP: ret = sge::KEY_HELP; break;
	case SDLK_MENU: ret = sge::KEY_MENU; break;
	case SDLK_SELECT: ret = sge::KEY_SELECT; break;
	case SDLK_STOP: ret = sge::KEY_STOP; break;
	case SDLK_AGAIN: ret = sge::KEY_AGAIN; break;
	case SDLK_UNDO: ret = sge::KEY_UNDO; break;
	case SDLK_CUT: ret = sge::KEY_CUT; break;
	case SDLK_COPY: ret = sge::KEY_COPY; break;
	case SDLK_PASTE: ret = sge::KEY_PASTE; break;
	case SDLK_FIND: ret = sge::KEY_FIND; break;
	case SDLK_MUTE: ret = sge::KEY_MUTE; break;
	case SDLK_VOLUMEUP: ret = sge::KEY_VOLUMEUP; break;
	case SDLK_VOLUMEDOWN: ret = sge::KEY_VOLUMEDOWN; break;
	case SDLK_KP_COMMA: ret = sge::KEY_KP_COMMA; break;
	case SDLK_KP_EQUALSAS400: ret = sge::KEY_KP_EQUALSAS400; break;
	case SDLK_ALTERASE: ret = sge::KEY_ALTERASE; break;
	case SDLK_SYSREQ: ret = sge::KEY_SYSREQ; break;
	case SDLK_CANCEL: ret = sge::KEY_CANCEL; break;
	case SDLK_CLEAR: ret = sge::KEY_CLEAR; break;
	case SDLK_PRIOR: ret = sge::KEY_PRIOR; break;
	case SDLK_RETURN2: ret = sge::KEY_RETURN2; break;
	case SDLK_SEPARATOR: ret = sge::KEY_SEPARATOR; break;
	case SDLK_OUT: ret = sge::KEY_OUT; break;
	case SDLK_OPER: ret = sge::KEY_OPER; break;
	case SDLK_CLEARAGAIN: ret = sge::KEY_CLEARAGAIN; break;
	case SDLK_CRSEL: ret = sge::KEY_CRSEL; break;
	case SDLK_EXSEL: ret = sge::KEY_EXSEL; break;
	case SDLK_THOUSANDSSEPARATOR: ret = sge::KEY_THOUSANDSSEPARATOR; break;
	case SDLK_DECIMALSEPARATOR: ret = sge::KEY_DECIMALSEPARATOR; break;
	case SDLK_CURRENCYUNIT: ret = sge::KEY_CURRENCYUNIT; break;
	case SDLK_CURRENCYSUBUNIT: ret = sge::KEY_CURRENCYSUBUNIT; break;
	case SDLK_KP_LEFTPAREN: ret = sge::KEY_KP_LEFTPAREN; break;
	case SDLK_KP_RIGHTPAREN: ret = sge::KEY_KP_RIGHTPAREN; break;
	case SDLK_KP_LEFTBRACE: ret = sge::KEY_KP_LEFTBRACE; break;
	case SDLK_KP_RIGHTBRACE: ret = sge::KEY_KP_RIGHTBRACE; break;
	case SDLK_KP_TAB: ret = sge::KEY_KP_TAB; break;
	case SDLK_KP_BACKSPACE: ret = sge::KEY_KP_BACKSPACE; break;
	case SDLK_KP_A: ret = sge::KEY_KP_A; break;
	case SDLK_KP_B: ret = sge::KEY_KP_B; break;
	case SDLK_KP_C: ret = sge::KEY_KP_C; break;
	case SDLK_KP_D: ret = sge::KEY_KP_D; break;
	case SDLK_KP_E: ret = sge::KEY_KP_E; break;
	case SDLK_KP_F: ret = sge::KEY_KP_F; break;
	case SDLK_KP_XOR: ret = sge::KEY_KP_XOR; break;
	case SDLK_KP_POWER: ret = sge::KEY_KP_POWER; break;
	case SDLK_KP_PERCENT: ret = sge::KEY_KP_PERCENT; break;
	case SDLK_KP_LESS: ret = sge::KEY_KP_LESS; break;
	case SDLK_KP_GREATER: ret = sge::KEY_KP_GREATER; break;
	case SDLK_KP_AMPERSAND: ret = sge::KEY_KP_AMPERSAND; break;
	case SDLK_KP_DBLAMPERSAND: ret = sge::KEY_KP_DBLAMPERSAND; break;
	case SDLK_KP_VERTICALBAR: ret = sge::KEY_KP_VERTICALBAR; break;
	case SDLK_KP_DBLVERTICALBAR: ret = sge::KEY_KP_DBLVERTICALBAR; break;
	case SDLK_KP_COLON: ret = sge::KEY_KP_COLON; break;
	case SDLK_KP_HASH: ret = sge::KEY_KP_HASH; break;
	case SDLK_KP_SPACE: ret = sge::KEY_KP_SPACE; break;
	case SDLK_KP_AT: ret = sge::KEY_KP_AT; break;
	case SDLK_KP_EXCLAM: ret = sge::KEY_KP_EXCLAM; break;
	case SDLK_KP_MEMSTORE: ret = sge::KEY_KP_MEMSTORE; break;
	case SDLK_KP_MEMRECALL: ret = sge::KEY_KP_MEMRECALL; break;
	case SDLK_KP_MEMCLEAR: ret = sge::KEY_KP_MEMCLEAR; break;
	case SDLK_KP_MEMADD: ret = sge::KEY_KP_MEMADD; break;
	case SDLK_KP_MEMSUBTRACT: ret = sge::KEY_KP_MEMSUBTRACT; break;
	case SDLK_KP_MEMMULTIPLY: ret = sge::KEY_KP_MEMMULTIPLY; break;
	case SDLK_KP_MEMDIVIDE: ret = sge::KEY_KP_MEMDIVIDE; break;
	case SDLK_KP_PLUSMINUS: ret = sge::KEY_KP_PLUSMINUS; break;
	case SDLK_KP_CLEAR: ret = sge::KEY_KP_CLEAR; break;
	case SDLK_KP_CLEARENTRY: ret = sge::KEY_KP_CLEARENTRY; break;
	case SDLK_KP_BINARY: ret = sge::KEY_KP_BINARY; break;
	case SDLK_KP_OCTAL: ret = sge::KEY_KP_OCTAL; break;
	case SDLK_KP_DECIMAL: ret = sge::KEY_KP_DECIMAL; break;
	case SDLK_KP_HEXADECIMAL: ret = sge::KEY_KP_HEXADECIMAL; break;
	case SDLK_LCTRL: ret = sge::KEY_LCTRL; break;
	case SDLK_LSHIFT: ret = sge::KEY_LSHIFT; break;
	case SDLK_LALT: ret = sge::KEY_LALT; break;
	case SDLK_LGUI: ret = sge::KEY_LGUI; break;
	case SDLK_RCTRL: ret = sge::KEY_RCTRL; break;
	case SDLK_RSHIFT: ret = sge::KEY_RSHIFT; break;
	case SDLK_RALT: ret = sge::KEY_RALT; break;
	case SDLK_RGUI: ret = sge::KEY_RGUI; break;
	case SDLK_MODE: ret = sge::KEY_MODE; break;
	case SDLK_AUDIONEXT: ret = sge::KEY_AUDIONEXT; break;
	case SDLK_AUDIOPREV: ret = sge::KEY_AUDIOPREV; break;
	case SDLK_AUDIOSTOP: ret = sge::KEY_AUDIOSTOP; break;
	case SDLK_AUDIOPLAY: ret = sge::KEY_AUDIOPLAY; break;
	case SDLK_AUDIOMUTE: ret = sge::KEY_AUDIOMUTE; break;
	case SDLK_MEDIASELECT: ret = sge::KEY_MEDIASELECT; break;
	case SDLK_WWW: ret = sge::KEY_WWW; break;
	case SDLK_MAIL: ret = sge::KEY_MAIL; break;
	case SDLK_CALCULATOR: ret = sge::KEY_CALCULATOR; break;
	case SDLK_COMPUTER: ret = sge::KEY_COMPUTER; break;
	case SDLK_AC_SEARCH: ret = sge::KEY_AC_SEARCH; break;
	case SDLK_AC_HOME: ret = sge::KEY_AC_HOME; break;
	case SDLK_AC_BACK: ret = sge::KEY_AC_BACK; break;
	case SDLK_AC_FORWARD: ret = sge::KEY_AC_FORWARD; break;
	case SDLK_AC_STOP: ret = sge::KEY_AC_STOP; break;
	case SDLK_AC_REFRESH: ret = sge::KEY_AC_REFRESH; break;
	case SDLK_AC_BOOKMARKS: ret = sge::KEY_AC_BOOKMARKS; break;
	case SDLK_BRIGHTNESSDOWN: ret = sge::KEY_BRIGHTNESSDOWN; break;
	case SDLK_BRIGHTNESSUP: ret = sge::KEY_BRIGHTNESSUP; break;
	case SDLK_DISPLAYSWITCH: ret = sge::KEY_DISPLAYSWITCH; break;
	case SDLK_KBDILLUMTOGGLE: ret = sge::KEY_KBDILLUMTOGGLE; break;
	case SDLK_KBDILLUMDOWN: ret = sge::KEY_KBDILLUMDOWN; break;
	case SDLK_KBDILLUMUP: ret = sge::KEY_KBDILLUMUP; break;
	case SDLK_EJECT: ret = sge::KEY_EJECT; break;
	case SDLK_SLEEP: ret = sge::KEY_SLEEP; break;
	}

	return ret;
}

Game::Game(void)
	: m_window(NULL)
	, m_window_visibled(false)
	, m_gl(NULL)
	, m_fs(NULL)
{
}

Game::~Game(void)
{
	if (m_window != NULL)
		Shutdown();
}

bool Game::Init(void)
{
	SGE_ASSERT(m_window == NULL);

	if (!m_game.init(NULL))
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

	m_window_id = SDL_GetWindowID(m_window);
	SDL_GetWindowPosition(m_window, &m_window_rect[0], &m_window_rect[1]);
	SDL_GetWindowSize(m_window, &m_window_rect[2], &m_window_rect[3]);

	m_gl = SDL_GL_CreateContext(m_window);
	if (m_gl == NULL)
		goto bad2;

	SDL_GL_MakeCurrent(m_window, m_gl);

	if (gl3wInit(&m_gl3w, (GL3WGetProcAddressProc)SDL_GL_GetProcAddress) != GL3W_OK)
		goto bad3;

	gl3wProcs = &m_gl3w;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	if (!m_glex.Init())
		goto bad3;

	if (!m_view.init(&m_glex))
		goto bad4;

	return true;

bad4:
	m_glex.Shutdown();

bad3:
	SDL_GL_DeleteContext(m_gl);
	m_gl = NULL;

bad2:
	SDL_DestroyWindow(m_window);
	m_window = NULL;

bad1:
	m_game.shutdown();

bad0:
	return false;
}

void Game::Shutdown(void)
{
	SGE_ASSERT(m_window != NULL);

	m_glex.Shutdown();

	SDL_GL_DeleteContext(m_gl);
	m_gl = NULL;

	SDL_DestroyWindow(m_window);
	m_window = NULL;

	m_game.shutdown();
}

bool Game::HandleEvent(const SDL_Event *event)
{
	if (event == NULL)
		return false;

	bool ret;

	switch (event->type) {
	case SDL_KEYDOWN:
	case SDL_KEYUP:
		ret = HandleKeyEvent(event);
		break;
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		ret = HandleMouseButtonEvent(event);
		break;
	case SDL_MOUSEMOTION:
		ret = HandleMouseMoveEvent(event);
		break;
	case SDL_WINDOWEVENT:
		ret = HandleWindowEvent(event);
		break;
	default:
		ret = false;
		break;
	}

	return ret;
}

void Game::Frame(float elapsed)
{
	m_game.update(elapsed);

	if (m_window_visibled) {
		glViewport(0, 0, m_window_rect[2], m_window_rect[3]);
		glClear(GL_COLOR_BUFFER_BIT);

		m_glex.BeginFrame();

		m_game.draw(&m_view);
		m_glex.EndFrame();

		SDL_GL_SwapWindow(m_window);
	}
}

bool Game::HandleKeyEvent(const SDL_Event* event)
{
	sge::event evt;
	
	switch (event->type) {
	case SDL_KEYDOWN:
		evt.type = sge::event::TYPE_KEY_DOWN;
		break;
	case SDL_KEYUP:
		evt.type = sge::event::TYPE_KEY_UP;
		break;
	default:
		SGE_ASSERT(false);
		break;
	}

	evt.value.v_key.keycode = MapKey(event->key.keysym.sym);
	if (evt.value.v_key.keycode == sge::KEY_UNKNOWN)
		return false;

	return m_game.handle_event(&evt);
}

bool Game::HandleMouseButtonEvent(const SDL_Event* event)
{
	sge::event evt;
	
	switch (event->type) {
	case SDL_KEYDOWN:
		evt.type = sge::event::TYPE_MOUSE_BUTTON_UP;
		break;
	case SDL_KEYUP:
		evt.type = sge::event::TYPE_MOUSE_BUTTON_UP;
		break;
	default:
		SGE_ASSERT(false);
		break;
	}

	switch (event->button.button) {
	case SDL_BUTTON_LEFT:
		evt.value.v_mouse_button.button = sge::event::mouse_button::BUTTON1;
		break;
	case SDL_BUTTON_MIDDLE:
		evt.value.v_mouse_button.button = sge::event::mouse_button::BUTTON3;
		break;
	case SDL_BUTTON_RIGHT:
		evt.value.v_mouse_button.button = sge::event::mouse_button::BUTTON2;
		break;
	default:
		return false;
	}

	return m_game.handle_event(&evt);
}

bool Game::HandleMouseMoveEvent(const SDL_Event* event)
{
	SGE_ASSERT(event->type == SDL_MOUSEMOTION);

	sge::event evt(sge::event::TYPE_MOUSE_MOVE);

	evt.value.v_mouse_move.dx = event->motion.xrel;
	evt.value.v_mouse_move.dy = event->motion.yrel;

	return m_game.handle_event(&evt);
}

bool Game::HandleWindowEvent(const SDL_Event* event)
{
	SGE_ASSERT(event->type == SDL_WINDOWEVENT);

	switch (event->window.event) {
	case SDL_WINDOWEVENT_MOVED:
		m_window_rect[0] = event->window.data1;
		m_window_rect[1] = event->window.data2;
		break;
	case SDL_WINDOWEVENT_RESIZED:
		m_window_rect[2] = event->window.data1;
		m_window_rect[3] = event->window.data2;
		break;
	case SDL_WINDOWEVENT_SHOWN:
		m_window_visibled = true;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		m_window_visibled = false;
		break;
	default:
		return false;
	}

	return true;
}