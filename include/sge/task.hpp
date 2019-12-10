//
//
#ifndef SGE_TASK_HPP
#define SGE_TASK_HPP

#include <sge/common.hpp>

SGE_BEGIN

class Task {
public:
	Task(lua_State *L);

public:
	lua_State *toLua(void);

private:
	lua_State *m_L;
};

inline lua_State *Task::toLua(void)
{
	return m_L;
}

SGE_END

#endif // SGE_TASK_HPP

