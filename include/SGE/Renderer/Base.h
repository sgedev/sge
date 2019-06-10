//
//
#ifndef SGE_RENDERER_BASE_H
#define SGE_RENDERER_BASE_H

#include <QObject>
#include <QRect>
#include <QMatrix4x4>

#include <SGE/Renderer/Common.h>

SGE_RENDERER_BEGIN

class Base: public QObject {
	Q_OBJECT

public:
	Base(QObject *parent = 0);
	virtual ~Base(void);

public:
	const QRect &viewport(void) const;
	void setViewport(int x, int y, int width, int height);
	void setViewport(const QRect &rect);
	virtual void reset(void);
	virtual void render(void);
	// void draw(...);

protected:
	bool init(void);

private:
	QRect m_viewport;
	QMatrix4x4 m_viewMatrix;
	QMatrix4x4 m_projectionMatrix;
};

inline const QRect &Base::viewport(void) const
{
	return m_viewport;
}

inline void Base::setViewport(int x, int y, int width, int height)
{
	setViewport(QRect(x, y, width, height));
}

inline void Base::setViewport(const QRect &rect)
{
	m_viewport = rect.normalized();
}

inline bool Base::init(void)
{
	m_viewMatrix.setToIdentity();
	m_projectionMatrix.fill(0.0f);

	return true;
}

SGE_RENDERER_END

#endif // SGE_RENDERER_BASE_H

