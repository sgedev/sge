//
//
#ifndef SGE_GUI_WINDOW_HPP
#define SGE_GUI_WINDOW_HPP

#include <string>

#include <sge/noncopyable.hpp>
#include <sge/gui/common.hpp>

SGE_GUI_BEGIN

class window: public noncopyable {
public:
	window(void);
	virtual ~window(void);

public:
	virtual bool create(const char *name, int x, int y, int width, int height, int flags = 0, window *parent = NULL);
	virtual void destroy(void);
	virtual void update(float elapsed);
	void move(int x, int y);
	void resize(int width, int height);
	window *parent(void);
	window *first_child(void);
	window *next_sibling(void);

private:
	void set_parent(window *parent);

private:
	window *m_parent;
	window *m_prev;
	window *m_next;
	window *m_first_child;
	glm::ivec4 m_rect;
	int m_flags;
	std::string m_name;
	bool m_created;
};

inline void window::move(int x, int y)
{
	m_rect[0] = x;
	m_rect[1] = y;
}

inline void window::resize(int width, int height)
{
	m_rect[2] = width;
	m_rect[3] = height;
}

inline window *window::parent(void)
{
	return m_parent;
}

inline window *window::first_child(void)
{
	return m_first_child;
}

inline window *window::next_sibling(void)
{
	return m_next;
}

SGE_GUI_END

#endif // SGE_GUI_WINDOW_HPP

