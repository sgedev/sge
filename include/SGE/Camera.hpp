//
//
#ifndef SGE_CAMERA_HPP
#define SGE_CAMERA_HPP

#include <QRect>
#include <QMatrix4x4>

#include <SGE/Common.hpp>
#include <SGE/Object.hpp>

SGE_BEGIN

class Q_DECL_EXPORT Camera: public Object {
	Q_OBJECT
	Q_PROPERTY(QRect viewport MEMBER m_viewport NOTIFY viewportChanged)
	Q_PROPERTY(QMatrix4x4 projection MEMBER m_projection NOTIFY projectionChanged)

public:
	Camera(Object *parent = Q_NULLPTR);
	virtual ~Camera(void);

signals:
	void viewportChanged(const QRect &v);
	void projectionChanged(const QMatrix4x4 &v);

private:
	QRect m_viewport;
	QMatrix4x4 m_projection;
};

SGE_END

#endif // SGE_CAMERA_HPP
