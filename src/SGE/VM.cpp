//
//

#include <SGE/VM.hpp>

SGE_BEGIN

VM::VM(QObject *parent):
	QThread(parent),
	m_trapHandler(Q_NULLPTR)
{
}

VM::~VM(void)
{
}

void VM::run(void)
{
	QThread::exec();
}

void VM::setTrapHandler(TrapHandler *p)
{
	m_trapHandler = p;
}

SGE_END
