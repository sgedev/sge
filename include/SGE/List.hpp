//
//
#ifndef SGE_LIST_HPP
#define SGE_LIST_HPP

#include <SGE/Common.hpp>

SGE_BEGIN

template <typename T>
class List final: public Noncopyable {
public:
	class Node final: public Noncopyable {
	public:
		Node(T *value = NULL) :
			m_value(value),
			m_prev(this),
			m_next(this)
		{
		}

		~Node(void)
		{
			unlink();
		}

	public:
		T *value(void)
		{
			return m_value;
		}

		const T *value(void) const
		{
			return m_value;
		}

		Node *prev(void)
		{
			return m_prev;
		}

		const Node *prev(void) const
		{
			return m_prev;
		}

		Node *next(void)
		{
			return m_next;
		}

		const Node *next(void) const
		{
			return m_next;
		}

		void link(Node *prev, Node *next)
		{
			SGE_ASSERT(prev != NULL);
			SGE_ASSERT(next != NULL);
			m_prev = prev;
			m_next = next;
			prev->m_next = this;
			next->m_prev = this;
		}

		void unlink(void)
		{
			m_prev->m_next = m_next;
			m_next->m_prev = m_prev;
			m_prev = m_next = this;
		}

		bool linked(void) const
		{
			return m_next != this;
		}

	private:
		T *m_value;
		Node *m_prev;
		Node *m_next;
	};

public:
	List(void)
	{
	}

	~List(void)
	{
		clear();
	}

public:
	bool empty(void) const
	{
		return !m_knot.linked();
	}

	int size(void) const
	{
		int n = 0;
		for (const Node *p(first()); p != knot(); p = p->next())
			n += 1;
		return n;
	}

	Node *knot(void)
	{
		return &m_knot;
	}

	const Node *knot(void) const
	{
		return &m_knot;
	}

	Node *first(void)
	{
		return m_knot.next();
	}

	const Node *first(void) const
	{
		return m_knot.next();
	}

	Node *last(void)
	{
		return m_knot.prev();
	}

	const Node *last(void) const
	{
		return m_knot.prev();
	}

	void prepand(Node *node)
	{
		SGE_ASSERT(node != NULL);
		node->link(knot(), first());
	}

	void append(Node *node)
	{
		SGE_ASSERT(node != NULL);
		node->link(last(), knot());
	}

	void removeFirst(void)
	{
		first()->unlink();
	}

	void removeLast(void)
	{
		last()->unlink();
	}

	void remove(Node *node)
	{
		SGE_ASSERT(node != NULL);
		SGE_ASSERT(contains(node));
		node->unlink();
	}

	void clear(void)
	{
		while (!empty())
			removeFirst();
	}

	bool contains(const Node *node) const
	{
		SGE_ASSERT(node != NULL);
		for (const Node *p = first(); p != knot(); p = p->next()) {
			if (p == node)
				return true;
		}
		return false;
	}

private:
	Node m_knot;
};

SGE_END

#endif // SGE_LIST_HPP
