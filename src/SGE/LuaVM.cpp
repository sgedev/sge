//
//

#include <SGE/LuaVM.hpp>

SGE_BEGIN

LuaVM::LuaVM(QObject *parent):
	VM(parent)
{
}

LuaVM::~LuaVM(void)
{
}

void LuaVM::run(void)
{
	VM::run();
}

SGE_END
