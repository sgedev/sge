//
//
#ifndef SGE_GUI_WINDOW_HPP
#define SGE_GUI_WINDOW_HPP

#include <string>

#include <sge/signal.hpp>
#include <sge/gui/common.hpp>
#include <sge/gui/node.hpp>

SGE_GUI_BEGIN

class window: public node {
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
	bool create(const char *name, int x, int y, int width, int height,
		window *parent, int flags = FLAG_VISIBLED);

	void destroy(void);
	virtual void update(void);
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
	int flags(void) const;

public:
	struct {
		signal<> shown;
		signal<> hidden;
		signal<int, int> move;
		signal<int, int> resize;
		signal<const char *> name_changed;
	} signals;

protected:
	void set_flags(int flags);

private:
	enum {
		INTERNAL_FLAG_ENABLED = (1 << 0),
		INTERNAL_FLAG_VISIBLED = (1 << 1)
	};

private:
	glm::ivec4 m_rect;
	int m_flags;
	int m_imgui_flags;
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
	signals.shown.emit();
}

inline void window::hide(void)
{
	if (!(m_internal_flags & INTERNAL_FLAG_VISIBLED))
		return;

	m_internal_flags &= ~INTERNAL_FLAG_VISIBLED;
	signals.hidden.emit();
}

inline const glm::ivec4 &window::rect(void) const
{
	return m_rect;
}

inline void window::set_rect(int x, int y, int width, int height)
{
	resize(width, height);
	move(x, y);
}

inline void window::set_rect(const glm::ivec4 &rc)
{
	resize(rc[2], rc[3]);
	move(rc[0], rc[1]);
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

	signals.move.emit(x, y);
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

	signals.resize.emit(width, height);
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
	// TODO signals.name_changed(name);
}

inline int window::flags(void) const
{
	return m_flags;
}

SGE_GUI_END

#endif // SGE_GUI_WINDOW_HPP

