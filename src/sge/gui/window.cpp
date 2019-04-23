//
//
#include <sge/gui/window.hpp>

SGE_GUI_BEGIN

window::list::node::node(void)
{
	reset();
}

window::list::node::~node(void)
{
	unlink();
}

window::list::node::reset(void)
{
	m_prev = this;
	m_next = this;
}

void window::list::node::link(node *prev, node *next)
{
	SGE_ASSERT(m_prev != NULL);
	SGE_ASSERT(m_next != NULL);
	SGE_ASSERT(prev != NULL);
	SGE_ASSERT(next != NULL);

	m_prev = prev;
	m_next = next;
	prev->next = this;
	next->prev = this;
}

void window::list::node::unlink(void)
{
	SGE_ASSERT(m_prev != NULL);
	SGE_ASSERT(m_next != NULL);

	m_prev->next = m_next;
	m_next->prev = m_prev;

	reset();
}

bool window::list::node::is_linked(void) const
{
	SGE_ASSERT(m_prev != NULL);
	SGE_ASSERT(m_next != NULL);

	return (m_prev != this);
}

window::list::node *window::list::node::get_prev(void)
{
	SGE_ASSERT(m_prev != NULL);
	SGE_ASSERT(m_next != NULL);

	return m_prev;
}

window::list::node *window::list::node::get_next(void)
{
	SGE_ASSERT(m_prev != NULL);
	SGE_ASSERT(m_next != NULL);

	return m_next;
}

window::list::list(void)
	: m_length(0)
{
}

window::list::~list(void)
{
	while (!is_empty())
		del_head();
}

void window::list::reset(void)
{
	m_knot.reset();
	m_length = 0;
}

bool window::list::is_empty(void) const
{
	return (get_length() == 0);
}

size_t window::list::get_length(void) const
{
	return m_length;
}

window::list::node *window::list::get_knot(void)
{
	return &m_knot;
}

const window::list::node *window::list::get_knot(void) const
{
	return &m_knot;
}

window::list::node *window::list::get_head(void)
{
	return m_knot->get_next();
}

const window::list::node *window::list::get_head(void) const
{
	return m_knot->get_next();
}

list::node *list::get_tail(void)
{
	return m_knot->get_prev();
}

const window::list::node *window::list::get_tail(void) const
{
	return m_knot->get_prev();
}

void window::list::add_head(window::list::node *p)
{
	SGE_ASSERT(p != NULL);

	p->link(get_knot(), get_head());
	m_length++;
}

void window::list::add_tail(window::list::node *p)
{
	SGE_ASSERT(p != NULL);

	p->link(get_tail(), get_knot());
	m_length++;
}

window::list::node *window::list::remove_head(void)
{
	list::node *p = get_head();
	p->unlink();
	m_length--;
	return p;
}

window::list::node *window::list::remove_tail(void)
{
	node *p = get_tail();
	p->unlink();
	m_length--;
	return p;
}

void window::list::remove(window::list::node *p)
{
	SGE_ASSERT(p != NULL);
	SGE_ASSERT(p->is_linked());
	SGE_ASSERT(has(p));

	p->unlink();
	m_length--;
}

bool window::list::has(const window::list::node *p) const
{
	SGE_ASSERT(p != NULL);

	if (!p->is_linked())
		return false;

	for (node *n = get_head(); n != get_knot(); n = n->get_next()) {
		if (p == n)
			return true;
	}

	return false;
}

window::window(void)
	: m_rect(0, 0, 0, 0)
	, m_parent(NULL)
	, m_flags(0)
{
}

window::~window(void)
{
}

bool window::init(window *parent, const char *name, int x, int y, int width, int height, int flags)
{
	m_name = name;
	m_flags = flags;
	m_parent = parent;

	m_rect[0] = x;
	m_rect[1] = y;
	m_rect[2] = width;
	m_rect[3] = height;

	if (m_parent != NULL)
		m_parent->m_children.add_tail(&m_node);

	return true;
}

void window::shutdown(void)
{
}

void window::update(float elapsed)
{
}

void window::draw(void)
{
}

window *get_parent(void);
window *first_child(void);
window *next_sibling(void);


SGE_GUI_END

