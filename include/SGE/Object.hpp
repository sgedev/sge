//
//
#ifndef SGE_OBJECT_HPP
#define SGE_OBJECT_HPP

#include <QObject>
#include <QSharedPointer>
#include <QVector3D>
#include <QQuaternion>

#include <SGE/Common.hpp>

SGE_BEGIN

class Q_DECL_EXPORT Object: public QObject {
	Q_OBJECT
	Q_PROPERTY(QVector3D position MEMBER m_position NOTIFY positionChanged);
	Q_PROPERTY(QVector3D scale MEMBER m_scale NOTIFY scaleChanged);
	Q_PROPERTY(QQuaternion rotation MEMBER m_rotation NOTIFY rotationChanged);

public:
	Object(Object *parent = Q_NULLPTR);
	virtual ~Object(void);

signals:
	void positionChanged(const QVector3D &v);
	void scaleChanged(const QVector3D &v);
	void rotationChanged(const QQuaternion &v);

private:
	QVector3D m_position;
	QVector3D m_scale;
	QQuaternion m_rotation;
};

typedef QSharedPointer<Object> ObjectPtr;

SGE_END

#endif // SGE_OBJECT_HPP
