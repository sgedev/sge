//
//
#ifndef SGE_LUAVM_HPP
#define SGE_LUAVM_HPP

#include <SGE/Common.hpp>
#include <SGE/VM.hpp>

SGE_BEGIN

class LuaVM: public VM {
	Q_OBJECT

public:
	LuaVM(QObject *parent = Q_NULLPTR);
	virtual ~LuaVM(void);

protected:
	void run(void) Q_DECL_OVERRIDE;
};

SGE_END

#endif // SGE_LUAVM_HPP
