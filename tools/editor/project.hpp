//
//
#ifndef SGE_EDITOR_PROJECT_HPP
#define SGE_EDITOR_PROJECT_HPP

#include <sge.hpp>

#include <QList>

class Project: public SGE::Game {
public:
	Project(void);
	virtual ~Project(void);

public:
	void update(void);

protected:
	int trapFps(lua_State *L) override;
	int trapEditorIsEnabled(lua_State *L) override;
};

#endif // SGE_EDITOR_PROJECT_HPP
