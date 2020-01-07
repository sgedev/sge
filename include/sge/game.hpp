//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <QString>
#include <QList>
#include <QSharedPointer>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QTimer>

#include <sge/common.hpp>
#include <sge/scene.hpp>
#include <sge/camera.hpp>

SGE_BEGIN

class Game: public QThread {
	Q_OBJECT

public:
	Game(void);
	virtual ~Game(void);

public:
	Scene &scene(void);
	void update(float elapsed);

private:
	Scene m_scene;
};

inline Scene &Game::scene(void)
{
	return m_scene;
}

SGE_END

#endif // SGE_GAME_HPP

