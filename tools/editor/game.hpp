//
//
#ifndef SGE_EDITOR_GAME_HPP
#define SGE_EDITOR_GAME_HPP

#include <QEvent>

#include <sge/game.hpp>

class Game: public SGE::Game {
public:
	Game(void);
	virtual ~Game(void);

public:
	bool init(const std::string &root) override;
	void shutdown(void) override;
	bool handleEvent(const QEvent *evt);
	void update(float elapsed) override;
	void draw(SGE::View *v) override;

protected:
	int trapFps(lua_State *L) override;
	int trapEditorIsEnabled(lua_State *L) override;
};

#endif // SGE_EDITOR_GAME_HPP

