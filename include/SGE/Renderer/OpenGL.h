//
//
#ifndef SGE_RENDERER_OPENGL_H
#define SGE_RENDERER_OPENGL_H

#include <QObject>
#include <QMatrix4x4>
#include <QSurface>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>

#include <SGE/Renderer/Common.h>
#include <SGE/Renderer/Base.h>

SGE_RENDERER_BEGIN

class OpenGL: public Base {
	Q_OBJECT

public:
	OpenGL(QObject *parent = 0);
	virtual ~OpenGL(void);

public:
	bool create(QSurface *surface, QOpenGLContext *context);
	virtual void reset(void);
	virtual void render(void);

private:
	typedef QOpenGLFunctions_3_3_Core Functions;

private:
	QSurface *m_surface;
	QOpenGLContext *m_context;
	Functions *m_gl;
};

SGE_RENDERER_END

#endif // SGE_RENDERER_OPENGL_H

