//
//
#ifndef SGE_NODE_HPP
#define SGE_NODE_HPP

#include <QObject>
#include <QMatrix4x4>

#include <sge/common.hpp>

SGE_BEGIN

class Node: public QObject {
	Q_OBJECT

public:
	Node(Node *parent = Q_NULLPTR);
	virtual ~Node(void);

public:
	void enable(void);
	void disable(void);
	bool isEnabled(void) const;
	void show(void);
	void hide(void);
	bool isVisibled(void) const;
	const QMatrix4x4 &transform(void) const;
	QMatrix4x4 globalTransform(void) const;
	void setTransform(const QMatrix4x4 &v);
	virtual void update(float elapsed);

private:
	bool m_enabled;
	bool m_visibled;
	QMatrix4x4 m_transform;
};

inline void Node::enable(void)
{
	m_enabled = true;
}

inline void Node::disable(void)
{
	m_enabled = false;
}

inline bool Node::isEnabled(void) const
{
	return m_enabled;
}

inline void Node::show(void)
{
	m_visibled = true;
}

inline void Node::hide(void)
{
	m_visibled = false;
}

inline bool Node::isVisibled(void) const
{
	return m_visibled;
}

inline const QMatrix4x4 &Node::transform(void) const
{
	return m_transform;
}

inline void Node::setTransform(const QMatrix4x4 &v)
{
	m_transform = v;
}

SGE_END

#endif // SGE_NODE_HPP

