//
//
#ifndef SGE_EVENT_HPP
#define SGE_EVENT_HPP

#include <sge/common.hpp>
#include <sge/keycode.hpp>

SGE_BEGIN

struct Event final {
	enum {
		TYPE_INVALID = 0,

		TYPE_KEY_DOWN,
		TYPE_KEY_UP,

		TYPE_MOUSE_BUTTON_DOWN,
		TYPE_MOUSE_BUTTON_UP,
		TYPE_MOUSE_MOVE,

		TYPE_MAX
	};
	
	struct Key {
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
		Key v_key;
		MouseButton v_mouseButton;
		MouseMove v_mouseMove;
	} value;

	Event(int t = TYPE_INVALID);
	Event(const Event &that);
	Event &operator=(const Event &that);
};

inline Event::Event(int t)
	: type(t)
{
}

inline Event::Event(const Event& that)
	: type(that.type)
	, value(that.value)
{
}

inline Event &Event::operator=(const Event &that)
{
	if (this != &that) {
		type = that.type;
		value = that.value;
	}

	return (*this);
}

SGE_END

#endif // SGE_EVENT_HPP
