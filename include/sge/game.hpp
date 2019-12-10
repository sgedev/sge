//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <QList>
#include <QThread>
#include <QMutex>

#include <sge/common.hpp>
#include <sge/renderer.hpp>

SGE_BEGIN

class Game: public QThread {
	Q_OBJECT

public:
	enum State {
		StateIdle = 0,
		StateInitializing,
		StateLoading,
		StateReady,
		StatePlaying,
		StatePaused,
		StateFailed,
		StateExited
	};

public:
	Game(void);
	virtual ~Game(void);

signals:
	int trapFps(void);
	int trapMaxFps(void);
	int trapSetMaxFps(int v);
	//Mode trapMode(void);

public:
	void update(float elapsed);
	void draw(Renderer::View *view);
	State state(void) const;
	const QString &name(void) const;
	void rename(const QString &name);

protected:
	static JSValue jsFps(JSContext *ctx, JSValueConst thisVal, int argc, JSValueConst *argv);
	static JSValue jsMaxFps(JSContext *ctx, JSValueConst thisVal, int argc, JSValueConst *argv);
	static JSValue jsSetMaxFps(JSContext *ctx, JSValueConst thisVal, int argc, JSValueConst *argv);
	static int initExports(JSContext *ctx, JSModuleDef *m);
    void run(void) Q_DECL_OVERRIDE;

private:
	QString m_name;
	State m_state;
};

inline Game::State Game::state(void) const
{
	return m_state;
}

inline const QString &Game::name(void) const
{
	return m_name;
}

inline void Game::rename(const QString &name)
{
	m_name = name;
}

SGE_END

#endif // SGE_GAME_HPP

