//
//
#ifndef SGE_EVENT_HPP
#define SGE_EVENT_HPP

#include <sge/common.hpp>
#include <sge/key.hpp>

SGE_BEGIN

// event

class event {
public:
	enum {
		EVENT_INVALID = 0,

		EVENT_KEY,

		EVENT_MAX
	};


public:
	event(int t = EVENT_INVALID);
	event(const event &that);

public:
	int type(void) const;
	void set_type(int t);
	event &operator=(const event &that);

private:
	int m_type;
};

inline event::event(int t)
	: m_type(t)
{
}

inline event::event(const event &that)
	: m_type(that.m_type)
{
}

inline int event::type(void) const
{
	return m_type;
}

inline void event::set_type(int t)
{
	m_type = t;
}

// key_event

class key_event: public event {
public:
	key_event(void);

private:
	key m_key;
	int value;
};

inline key_event::key_event(void)
	: event(EVENT_KEY)
{
}

SGE_END

#endif // SGE_EVENT_HPP

