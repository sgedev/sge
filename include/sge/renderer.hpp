//
//
#ifndef SGE_RENDERER_HPP
#define SGE_RENDERER_HPP

#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>

#include <sge/common.hpp>
#include <sge/view.hpp>

SGE_BEGIN

class Renderer {
public:
	typedef QOpenGLFunctions_3_3_Core OpenGLFunctions;

public:
	Renderer(void);
	virtual ~Renderer(void);

public:
	bool init(void);
	void reset(void);
	void draw(View &view);

private:
	QOpenGLContext *m_context;
	OpenGLFunctions *m_gl;
};

SGE_END

#endif // SGE_RENDERER_HPP

