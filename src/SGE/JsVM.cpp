//
//

#include <SGE/JsVM.hpp>

SGE_BEGIN

JsVM::JsVM(QObject *parent):
	VM(parent)
{
}

JsVM::~JsVM(void)
{
}

void JsVM::run(void)
{
	VM::run();
}

SGE_END
