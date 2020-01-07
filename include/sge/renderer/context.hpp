//
//
#ifndef SGE_RENDERER_CONTEXT_HPP
#define SGE_RENDERER_CONTEXT_HPP

#include <QObject>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>

#include <sge/renderer/common.hpp>
#include <sge/renderer/state.hpp>
#include <sge/renderer/view.hpp>

SGE_RENDERER_BEGIN

class Context: public QObject {
	Q_OBJECT

public:
	typedef QOpenGLFunctions_3_3_Core OpenGLFunctions;

public:
	Context(QObject *parent = Q_NULLPTR);
	virtual ~Context(void);

public:
	bool init(void);
	void clear(void);
	void render(View *view);

private:
	QOpenGLContext *m_context;
	OpenGLFunctions *m_gl;
};

SGE_RENDERER_END

#endif // SGE_RENDERER_CONTEXT_HPP

