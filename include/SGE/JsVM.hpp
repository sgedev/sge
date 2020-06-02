//
//
#ifndef SGE_JSVM_HPP
#define SGE_JSVM_HPP

#include <SGE/Common.hpp>
#include <SGE/VM.hpp>

SGE_BEGIN

class JsVM: public VM {
	Q_OBJECT

public:
	JsVM(QObject *parent = Q_NULLPTR);
	virtual ~JsVM(void);

protected:
	void run(void) Q_DECL_OVERRIDE;
};

SGE_END

#endif // SGE_JSVM_HPP
