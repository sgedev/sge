//
//
#ifndef SGE_LIST_HPP
#define SGE_LIST_HPP

#include <sge/common.hpp>
#include <sge/noncopyable.hpp>

SGE_BEGIN

class list final: public noncopyable {
public:
	class node: public noncopyable {
	public:
		node(void);
		virtual ~node(void);

	public:
		void reset(void);
		void link(node *prev, node *next);
		void unlink(void);
		bool is_linked(void) const;
		node *get_prev(void);
		node *get_next(void);

	private:
		node *m_prev;
		node *m_next;
	};

public:
	list(void);
	~list(void);

public:
	void reset(void);
	bool is_empty(void) const;
	size_t get_length(void) const;
	node *get_knot(void);
	const node *get_knot(void) const;
	node *get_head(void);
	const node *get_head(void) const;
	node *get_tail(void);
	const node *get_tail(void) const;
	void add_head(node *p);
	void add_tail(node *p);
	node *remove_head(void);
	node *remove_tail(void);
	void remove(node *p);
	bool has(const node *p) const;

private:
	node m_knot;
	size_t m_length;
};

inline list::node::node(void)
{
	reset();
}

inline list::node::~node(void)
{
	unlink();
}

inline list::node::reset(void)
{
	m_prev = this;
	m_next = this;
}

inline void list::node::link(node *prev, node *next)
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

inline void list::node::unlink(void)
{
	SGE_ASSERT(m_prev != NULL);
	SGE_ASSERT(m_next != NULL);

	m_prev->next = m_next;
	m_next->prev = m_prev;

	reset();
}

inline bool list::node::is_linked(void) const
{
	SGE_ASSERT(m_prev != NULL);
	SGE_ASSERT(m_next != NULL);

	return (m_prev != this);
}

inline list::node *list::node::get_prev(void)
{
	SGE_ASSERT(m_prev != NULL);
	SGE_ASSERT(m_next != NULL);

	return m_prev;
}

inline list::node *list::node::get_next(void)
{
	SGE_ASSERT(m_prev != NULL);
	SGE_ASSERT(m_next != NULL);

	return m_next;
}

inline list::list(void)
	: m_length(0)
{
}

inline list::~list(void)
{
	while (!is_empty())
		del_head();
}

inline void list::reset(void)
{
	m_knot.reset();
	m_length = 0;
}

inline bool list::is_empty(void) const
{
	return (get_length() == 0);
}

inline size_t list::get_length(void) const
{
	return m_length;
}

inline list::node *list::get_knot(void)
{
	return &m_knot;
}

inline const list::node *list::get_knot(void) const
{
	return &m_knot;
}

inline node *list::get_head(void)
{
	return m_knot->get_next();
}

inline const list::node *list::get_head(void) const
{
	return m_knot->get_next();
}

inline list::node *list::get_tail(void)
{
	return m_knot->get_prev();
}

inline const list::node *list::get_tail(void) const
{
	return m_knot->get_prev();
}

inline void list::add_head(list::node *p)
{
	SGE_ASSERT(p != NULL);

	p->link(get_knot(), get_head());
	m_length++;
}

inline void list::add_tail(list::node *p)
{
	SGE_ASSERT(p != NULL);

	p->link(get_tail(), get_knot());
	m_length++;
}

inline list::node *list::remove_head(void)
{
	node *p = get_head();
	p->unlink();
	m_length--;
	return p;
}

inline list::node *list::remove_tail(void)
{
	node *p = get_tail();
	p->unlink();
	m_length--;
	return p;
}

inline void list::remove(list::node *p)
{
	SGE_ASSERT(p != NULL);
	SGE_ASSERT(p->is_linked());
	SGE_ASSERT(has(p));

	p->unlink();
	m_length--;
}

inline bool list::has(const list::node *p) const
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

SGE_END

#endif // SGE_LIST_HPP

