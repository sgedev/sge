//
//
#ifndef SGE_NODE_HPP
#define SGE_NODE_HPP

#include <QVector>
#include <QMatrix4x4>
#include <QObject>

#include <sge/common.hpp>
#include <sge/view.hpp>

SGE_BEGIN

class Node: public QObject {
	Q_OBJECT
	Q_PROPERTY(QMatrix4x4 transform READ transform WRITE setTransform NOTIFY transformChanged)

public:
	Node(QObject *parent = Q_NULLPTR);
	virtual ~Node(void);

signals:
	void transformChanged(void);
	void childAdded(Node *child);
	void childRemoved(Node *child);

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
	void update(float elapsed);
	void draw(View &view) const;

protected:
	virtual void childEvent(QChildEvent *event) override;
	virtual void updateNode(float elapsed);
	virtual void drawNode(View &view) const;

private:
	typedef QVector<Node *> NodeVector;

	NodeVector m_childrenCache;
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
	if (m_transform != v) {
		m_transform = v;
		transformChanged();
	}
}

inline void Node::update(float elapsed)
{
	if (m_enabled)
		updateNode(elapsed);
}

inline void Node::draw(View &view) const
{
	if (m_enabled && m_visibled)
		drawNode(view);
}

SGE_END

#endif // SGE_NODE_HPP

