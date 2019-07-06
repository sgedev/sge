//
//
#ifndef SGE_GUI_WIDGET_HPP
#define SGE_GUI_WIDGET_HPP

#include <string>

#include <sge/gui/common.hpp>

SGE_GUI_BEGIN

class widget {
public:
	enum {
		FLAG_ENABLED = 0x1,
		FLAG_VISIBLED = 0x2,
	};

public:
	widget(widget *parent = NULL);
	virtual ~widget(void);

public:
	const char *name(void) const;
	void set_name(const char *p);
	int flags(void) const;
	void set_flags(int v);
	widget *parent(void);
	void set_parent(widget *p);
	widget *first_child(void);
	widget *prev_sibling(void);
	widget *next_sibling(void);
	glm::vec2 pos(void) const;
	void move(float x, float y);
	glm::vec2 size(void) const;
	void resize(float width, float height);
	static void update_all(float elapsed);

protected:
	virtual void update(float elapsed);

private:
	void detach(void);

private:
	std::string m_name;
	glm::vec2 m_pos;
	glm::vec2 m_size;
	int m_flags;
	widget *m_parent;
	widget *m_prev_sibling;
	widget *m_next_sibling;
	widget *m_first_child;
	static widget *c_tops;
};

inline const char *widget::name(void) const
{
	return m_name.c_str();
}

inline void widget::set_name(const char *p)
{
	m_name = p;
}

inline int widget::flags(void) const
{
	return m_flags;
}

inline void widget::set_flags(int v)
{
	m_flags = v;
}

inline widget *widget::parent(void)
{
	return m_parent;
}

inline widget *widget::first_child(void)
{
	return m_first_child;
}

inline widget *widget::prev_sibling(void)
{
	return m_prev_sibling;
}

inline widget *widget::next_sibling(void)
{
	return m_next_sibling;
}

inline glm::vec2 widget::pos(void) const
{
	return m_pos;
}

inline void widget::move(float x, float y)
{
	m_pos.x = x;
	m_pos.y = y;
}

inline glm::vec2 widget::size(void) const
{
	return m_size;
}

inline void widget::resize(float width, float height)
{
	SGE_ASSERT(width > 0.0f);
	SGE_ASSERT(height > 0.0f);

	m_size.x = width;
	m_size.y = height;
}

SGE_GUI_END

#endif // SGE_GUI_WIDGET_HPP
