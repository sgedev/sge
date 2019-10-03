//
//
#ifndef SGE_EVENT_HPP
#define SGE_EVENT_HPP

#include <sge/common.hpp>
#include <sge/keycode.hpp>

SGE_BEGIN

struct event final {
	enum {
		TYPE_INVALID = 0,

		TYPE_KEY_DOWN,
		TYPE_KEY_UP,

		TYPE_MOUSE_BUTTON_DOWN,
		TYPE_MOUSE_BUTTON_UP,
		TYPE_MOUSE_MOVE,

		TYPE_MAX
	};
	
	struct key {
		int keycode;
	};

	struct mouse_button {
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

	struct mouse_move {
		int dx;
		int dy;
	};

	int type;

	union {
		key v_key;
		mouse_button v_mouse_button;
		mouse_move v_mouse_move;
	} value;

	event(int t = TYPE_INVALID);
	event(const event &that);
	event &operator=(const event &that);
};

inline event::event(int t)
	: type(t)
{
}

inline event::event(const event& that)
	: type(that.type)
	, value(that.value)
{
}

SGE_END

#endif // SGE_EVENT_HPP
