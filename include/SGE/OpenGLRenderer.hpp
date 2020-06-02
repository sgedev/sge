//
//
#ifndef SGE_OPENGLRENDERER_HPP
#define SGE_OPENGLRENDERER_HPP

#include <QObject>
#include <QOpenGLFunctions_3_3_Core>

#include <SGE/Common.hpp>
#include <SGE/Renderer.hpp>

SGE_BEGIN

typedef QOpenGLFunctions_3_3_Core OpenGLFunctions;

class Q_DECL_EXPORT OpenGLRenderer: public Renderer {
	Q_OBJECT

public:
	OpenGLRenderer(QObject *parent = Q_NULLPTR);
	virtual ~OpenGLRenderer(void);

public:
	bool init(OpenGLFunctions *funcs);
	void addObject(ObjectPtr obj) Q_DECL_OVERRIDE;
	void removeObject(ObjectPtr obj) Q_DECL_OVERRIDE;

private:
	OpenGLFunctions *m_funcs;
};

SGE_END

#endif // SGE_OPENGLRENDERER_HPP
