//
//
#ifndef SGE_CLIENT_HPP
#define SGE_CLIENT_HPP

#include <QObject>
#include <QThread>
#include <QMutex>

#include <ttvfs.h>

#include <sge/common.hpp>
#include <sge/scene.hpp>
#include <sge/renderer.hpp>

SGE_BEGIN

class Client: public QObject {
	Q_OBJECT

public:
	enum State {
		StateIdel = 0,
		StateInitializing,
		StateLoading,
		StateReady,
		StatePlaying,
		StatePaused,
		StateFailed,
		StateExited
	};

public:
	Client(QObject *parent = Q_NULLPTR);
	virtual ~Client(void);

public:
	virtual bool init(ttvfs::Root *root);
	virtual void shutdown(void);
	virtual void update(float elapsed);
	virtual void draw(Renderer::View *view);
	ttvfs::Root &root(void);
	Scene &scene(void);
	State state(void) const;

private:
	ttvfs::Root *m_root;
	Scene m_scene;
	State m_state;
};

inline ttvfs::Root &Client::root(void)
{
	Q_ASSERT(m_root != Q_NULLPTR);
	return (*m_root);
}

inline Scene &Client::scene(void)
{
	return m_scene;
}

inline Client::State Client::state(void) const
{
	return m_state;
}

SGE_END

#endif // SGE_CLIENT_HPP

