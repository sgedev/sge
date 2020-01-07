//
//
#ifndef SGE_CAMERA_HPP
#define SGE_CAMERA_HPP

#include <QMatrix4x4>

#include <sge/common.hpp>
#include <sge/view.hpp>
#include <sge/node.hpp>

SGE_BEGIN

class Camera: public Node {
	Q_OBJECT

public:
	Camera(Node *parent = Q_NULLPTR);
	virtual ~Camera(void);

public:
	void update(float elapsed) override;
	QMatrix4x4 viewMatrix(void) const;
	const QMatrix4x4 &projectMatrix(void) const;
	void setProjectMatrix(const QMatrix4x4 &v);
	const View &view(void) const;

protected:
	void drawNode(Node *node, float elapsed);

public:
	Node *m_rootNode;
	View m_view;
};

inline QMatrix4x4 Camera::viewMatrix(void) const
{
	return -transform();
}

inline const QMatrix4x4 &Camera::projectMatrix(void) const
{
	return m_view.projectMatrix();
}

inline void Camera::setProjectMatrix(const QMatrix4x4 &v)
{
	m_view.setProjectMatrix(v);
}

inline const View &Camera::view(void) const
{
	return m_view;
}

SGE_END

#endif // SGE_CAMERA_HPP

