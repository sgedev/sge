//
//
#ifndef SGE_EVENT_HPP
#define SGE_EVENT_HPP

#include <cx/noncopyable.hpp>

#include <sge/common.hpp>
#include <sge/keycode.hpp>

SGE_BEGIN

struct Event: public CX::Noncopyable {
	enum {
		TYPE_INVALID = 0,

		TYPE_KEY_DOWN,
		TYPE_KEY_UP,

		TYPE_MOUSE_BUTTON_DOWN,
		TYPE_MOUSE_BUTTON_UP,
		TYPE_MOUSE_MOVE,

		TYPE_MAX
	};
	
	struct Keyboard {
		int keycode;
	};

	struct MouseButton {
		enum {
			BUTTON1,
			BUTTON2,
			BUTTON3,
			BUTTON4,
			BUTTON5,
			BUTTON6,
			BUTTON7,
			BOTTON8
		};

		int button;
	};

	struct MouseMove {
		int dx;
		int dy;
	};

	int type;

	union {
		Keyboard keyboard;
		MouseButton mouseButton;
		MouseMove mouseMove;
	} value;

	Event(void);
};

inline Event::Event(void)
	: type(TYPE_INVALID)
{
}

SGE_END

#endif // SGE_EVENT_HPP
