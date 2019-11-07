//
//
#ifndef SGE_EDITOR_GAME_HPP
#define SGE_EDITOR_GAME_HPP

#include <QDir>
#include <QEvent>

#include <sge/client.hpp>
#include <sge/renderer.hpp>
#include <sge/editor/common.hpp>

SGE_EDITOR_BEGIN

class Game: public Client {
public:
	Game(QObject *parent = Q_NULLPTR);
	virtual ~Game(void);

public:
	bool init(SGE::FileSystem *fs) override;
	void shutdown(void) override;
	void update(float elapsed) override;
	void draw(Renderer::View *view) override;
};

SGE_EDITOR_END

#endif // SGE_EDITOR_GAME_HPP

