//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <QString>

#include <lua.hpp>

#include <sge/common.hpp>
#include <sge/vm.hpp>
#include <sge/renderer.hpp>

SGE_BEGIN

class Game: public VM {
	Q_OBJECT

public:
	struct Traps {
		virtual void trapInfo(const QString &text) = 0;
		virtual void trapWarning(const QString &text) = 0;
		virtual void trapError(const QString &text) = 0;
		virtual unsigned int trapFps(void) = 0;
		virtual unsigned int trapMaxFps(void) = 0;
		virtual void trapSetMaxFps(unsigned int v) = 0;
	};

public:
	Game(void);
	virtual ~Game(void);

public:
	const QString &dir(void) const;
	const QString &name(void) const;
	void setName(const QString &name);
	void handleTraps(Traps *traps);
	void update(float elapsed);
	void draw(Renderer::View *view);

signals:
	void nameChanged(const QString &name);

protected:
	bool initMainTask(lua_State *L) override;
	int trapInfo(lua_State *L) override;
	int trapWarning(lua_State *L) override;
	int trapError(lua_State *L) override;
	int trapName(lua_State *L) override;
	int trapSetName(lua_State *L) override;
	int trapFps(lua_State *L) override;
	int trapMaxFps(lua_State *L) override;
	int trapSetMaxFps(lua_State *L) override;

private:
	QString m_dir;
	QString m_name;
};

inline const QString &Game::dir(void) const
{
	return m_dir;
}

inline const QString &Game::name(void) const
{
	return m_name;
}

SGE_END

#endif // SGE_GAME_HPP
