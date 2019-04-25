//
//
#ifndef SGE_GUI_WINDOW_HPP
#define SGE_GUI_WINDOW_HPP

#include <string>

#include <sge/noncopyable.hpp>
#include <sge/signal.hpp>
#include <sge/gui/common.hpp>

SGE_GUI_BEGIN

class window: public noncopyable {
public:
	enum {
		FLAG_NO_TITLE = (1 << 0),
		FLAG_NO_MOVE = (1 << 2),
		FLAG_NO_RESIZE = (1 << 1),
		FLAG_NO_SCROLL = (1 << 3),
		FLAG_NO_COLLAPSE = (1 << 4),
		FLAG_NO_BACKGROUND = (1 << 5),
		FLAG_NO_FRONT = (1 << 7),
		FLAG_MENUBAR = (1 << 6),
		FLAG_VISIBLED = (1 << 8)
	};

public:
	window(void);
	virtual ~window(void);

public:
	bool create(const char *name, int x, int y, int width, int height, int flags = 0, window *parent = NULL);
	void destroy(void);
	void update(float elapsed);
	bool is_visibled(void) const;
	void show(void);
	void hide(void);
	const glm::ivec4 &rect(void) const;
	void set_rect(int x, int y, int width, int height);
	void set_rect(const glm::ivec4 &rc);
	glm::ivec2 pos(void) const;
	void move(int x, int y);
	void move(const glm::ivec2 &v);
	glm::ivec2 size(void) const;
	void resize(int width, int height);
	void resize(const glm::ivec2 &v);
	const char *name(void) const;
	void set_name(const char *name);
	window *parent(void);
	window *first_child(void);
	window *next_sibling(void);

public:
	signal<> on_show;
	signal<> on_hide;
	signal<int, int> on_move;
	signal<int, int> on_resize;

protected:
	virtual bool do_create(void);
	virtual void do_destroy(void);
	virtual void do_update(float elapsed);

private:
	void set_parent(window *parent);

private:
	enum {
		INTERNAL_FLAG_ENABLED = (1 << 0),
		INTERNAL_FLAG_VISIBLED = (1 << 1)
	};

	window *m_parent;
	window *m_prev;
	window *m_next;
	window *m_first_child;
	glm::ivec4 m_rect;
	int m_flags;
	int m_internal_flags;
	std::string m_name;
	bool m_created;
};

inline bool window::is_visibled(void) const
{
	return !!(m_internal_flags & INTERNAL_FLAG_VISIBLED);
}

inline void window::show(void)
{
	if (m_internal_flags & INTERNAL_FLAG_VISIBLED)
		return;

	m_internal_flags |= INTERNAL_FLAG_VISIBLED;
	on_show.emit();
}

inline void window::hide(void)
{
	if (!(m_internal_flags & INTERNAL_FLAG_VISIBLED))
		return;

	m_internal_flags &= ~INTERNAL_FLAG_VISIBLED;
	on_hide.emit();
}

inline const glm::ivec4 &window::rect(void) const
{
	return m_rect;
}

inline void window::set_rect(int x, int y, int width, int height)
{
	move(x, y);
	resize(width, height);
}

inline void window::set_rect(const glm::ivec4 &rc)
{
	move(rc[0], rc[1]);
	resize(rc[2], rc[3]);
}

inline glm::ivec2 window::pos(void) const
{
	return glm::ivec2(m_rect[0], m_rect[1]);
}

inline void window::move(int x, int y)
{
	if (m_rect[0] == x && m_rect[1] == y)
		return;

	m_rect[0] = x;
	m_rect[1] = y;

	on_move.emit(x, y);
}

inline void window::move(const glm::ivec2 &v)
{
	move(v[0], v[1]);
}

inline glm::ivec2 window::size(void) const
{
	return glm::ivec2(m_rect[2], m_rect[3]);
}

inline void window::resize(int width, int height)
{
	if (m_rect[2] == width && m_rect[3] == height)
		return;

	m_rect[2] = width;
	m_rect[3] = height;

	on_resize.emit(width, height);
}

inline void window::resize(const glm::ivec2 &v)
{
	resize(v[0], v[1]);
}

inline const char *window::name(void) const
{
	return m_name.c_str();
}

inline void window::set_name(const char *name)
{
	m_name = name;
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

