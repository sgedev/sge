//
//
#ifndef SGE_RENDERER_HPP
#define SGE_RENDERER_HPP

#include <QList>
#include <QObject>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>

#include <sge/common.hpp>
#include <sge/scene.hpp>
#include <sge/camera.hpp>
#include <sge/view.hpp>

SGE_BEGIN

typedef QList<Camera *> CameraList;

class Renderer: public QObject {
	Q_OBJECT

public:
	typedef QOpenGLFunctions_3_3_Core OpenGLFunctions;

public:
	Renderer(QObject *parent = Q_NULLPTR);
	virtual ~Renderer(void);

public:
	bool init(Scene *scene);
	void reset(void);
	void draw(void);

protected:
	void renderView(View &view);

protected slots:
	void nodeAdded(Node *node);
	void nodeRemoved(Node *node);

private:
	Scene *m_scene;
	QOpenGLContext *m_context;
	OpenGLFunctions *m_gl;
	CameraList m_cameraList;
	View m_view;
};

SGE_END

#endif // SGE_RENDERER_HPP

