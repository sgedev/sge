//
//
#ifndef SGE_GUI_NODE_HPP
#define SGE_GUI_NODE_HPP

#include <sge/noncopyable.hpp>
#include <sge/gui/common.hpp>

SGE_GUI_BEGIN

class node: public noncopyable {
public:
	node(void);
	virtual ~node(void);

public:
	void set_parent(node *parent);
	node *parent(void);
	node *first_child(void);
	node *next_sibling(void);
	virtual void update(void);
	static void update_all(void);

protected:
	void detach(void);

private:
	node *m_parent;
	node *m_prev;
	node *m_next;
	node *m_first_child;
	static node *c_first_node;
};

inline node *node::parent(void)
{
	return m_parent;
}

inline node *node::first_child(void)
{
	return m_first_child;
}

inline node *node::next_sibling(void)
{
	return m_next;
}

SGE_GUI_END

#endif // SGE_GUI_NODE_HPP

