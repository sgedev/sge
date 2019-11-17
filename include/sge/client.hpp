//
//
#ifndef SGE_CLIENT_HPP
#define SGE_CLIENT_HPP

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QDomDocument>

#include <sge/common.hpp>
#include <sge/filesystem.hpp>
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
	virtual bool init(FileSystem *fs);
	virtual void shutdown(void);
	virtual void update(float elapsed);
	virtual void draw(Renderer::View *view);

private:
	FileSystem *m_fs;
	Scene m_scene;
	State m_state;
};

SGE_END

#endif // SGE_CLIENT_HPP

