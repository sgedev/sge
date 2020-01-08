//
//
#ifndef SGE_CAMERA_HPP
#define SGE_CAMERA_HPP

#include <QMatrix4x4>

#include <sge/common.hpp>
#include <sge/node.hpp>

SGE_BEGIN

class Camera: public Node {
	Q_OBJECT

public:
	Camera(Node *parent = Q_NULLPTR);
	virtual ~Camera(void);

public:
	QMatrix4x4 viewMatrix(void) const;
	const QMatrix4x4 &projectMatrix(void) const;
	void setProjectMatrix(const QMatrix4x4 &v);
	View &view(void);
	bool canSee(const Node *target) const;

protected:
	void drawNode(View &view) const override;

private:
	QMatrix4x4 m_projectMatrix;
};

inline QMatrix4x4 Camera::viewMatrix(void) const
{
	return -transform();
}

inline const QMatrix4x4 &Camera::projectMatrix(void) const
{
	return m_projectMatrix;
}

inline void Camera::setProjectMatrix(const QMatrix4x4 &v)
{
	m_projectMatrix = v;
}

SGE_END

#endif // SGE_CAMERA_HPP

