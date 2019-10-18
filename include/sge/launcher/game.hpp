//
//
#ifndef SGE_LAUNCHER_GAME_HPP
#define SGE_LAUNCHER_GAME_HPP

#include <sge/client.hpp>
#include <sge/launcher/common.hpp>

SGE_LAUNCHER_BEGIN

class Game: public Client {
	Q_OBJECT

public:
	Game(QObject *parent = Q_NULLPTR);
	virtual ~Game(void);
};

SGE_LAUNCHER_END

#endif // SGE_LAUNCHER_GAME_HPP

