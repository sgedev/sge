//
//
#ifndef SGE_RENDERER_VIEW_HPP
#define SGE_RENDERER_VIEW_HPP

#include <QObject>
#include <QRect>
#include <QVector3D>
#include <QMatrix4x4>

#include <sge/renderer/common.hpp>

SGE_RENDERER_BEGIN

class View: public QObject {
	Q_OBJECT

public:
	View(QObject *parent = Q_NULLPTR);
	virtual ~View(void);

public:
	void clear(void);
	const QRect &viewport(void) const;
	void setViewport(int x, int y, int widht, int height);
	bool isClearEnabled(void) const;
	void setClearEnabled(bool v);
	const QVector3D &clearColor(void) const;
	void setClearColor(const QVector3D &rgb);
	void setClearColor(float red, float green, float blue);
	const QMatrix4x4 &viewMatrix(void) const;
	void setViewMatrix(const QMatrix4x4 &v);
	const QMatrix4x4 &projectMatrix(void) const;
	void setProjectMatrix(const QMatrix4x4 &v);

private:
	QRect m_viewport;
	bool m_clearEnabled;
	QVector3D m_clearColor;
	QMatrix4x4 m_viewMatrix;
	QMatrix4x4 m_projectMatrix;
};

inline const QRect &View::viewport(void) const
{
	return m_viewport;
}

inline void View::setViewport(int x, int y, int widht, int height)
{
	QRect v(x, y, widht, height);
	m_viewport = v.normalized();
}

inline bool View::isClearEnabled(void) const
{
	return m_clearEnabled;
}

inline void View::setClearEnabled(bool v)
{
	m_clearEnabled = v;
}

inline const QVector3D &View::clearColor(void) const
{
	return m_clearColor;
}

inline void View::setClearColor(const QVector3D &rgb)
{
	m_clearColor = rgb;
}

inline void View::setClearColor(float red, float green, float blue)
{
	setClearColor(QVector3D(red, green, blue));
}

inline const QMatrix4x4 &View::viewMatrix(void) const
{
	return m_viewMatrix;
}

inline void View::setViewMatrix(const QMatrix4x4 &v)
{
	m_viewMatrix = v;
}

inline const QMatrix4x4 &View::projectMatrix(void) const
{
	return m_projectMatrix;
}

inline void View::setProjectMatrix(const QMatrix4x4 &v)
{
	m_projectMatrix = v;
}

SGE_RENDERER_END

#endif // SGE_RENDERER_VIEW_HPP

