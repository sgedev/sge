//
//
#ifndef SGE_VIEW_HPP
#define SGE_VIEW_HPP

#include <QColor>
#include <QMatrix4x4>

#include <sge/common.hpp>

SGE_BEGIN

class View {
public:
	View(void);
	virtual ~View(void);

public:
	void reset(void);
	void enableClear(void);
	void disableClear(void);
	bool isClearEnabled(void) const;
	const QColor clearColor(void) const;
	void setClearColor(const QColor &color);
	const QMatrix4x4 &viewMatrix(void) const;
	void setViewMatrix(const QMatrix4x4 &v);
	const QMatrix4x4 &projectMatrix(void) const;
	void setProjectMatrix(const QMatrix4x4 &v);

private:
	bool m_clearEnabled;
	QColor m_clearColor;
	QMatrix4x4 m_viewMatrix;
	QMatrix4x4 m_projectMatrix;
};

inline void View::enableClear(void)
{
	m_clearEnabled = true;
}

inline void View::disableClear(void)
{
	m_clearEnabled = false;
}

inline bool View::isClearEnabled(void) const
{
	return m_clearEnabled;
}

inline const QColor View::clearColor(void) const
{
	return m_clearColor;
}

inline void View::setClearColor(const QColor &color)
{
	m_clearColor = color;
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

SGE_END

#endif // SGE_VIEW_HPP

