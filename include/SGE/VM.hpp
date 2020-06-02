//
//
#ifndef SGE_VM_HPP
#define SGE_VM_HPP

#include <QString>
#include <QThread>

#include <SGE/Common.hpp>

SGE_BEGIN

class VM: public QThread {
	Q_OBJECT

public:
	struct TrapHandler {
		typedef qintptr ObjectID;
		virtual ObjectID objectNew(const QString &name, ObjectID parent) = 0;
		virtual void objectDestroy(ObjectID obj) = 0;
	};

public:
	VM(QObject *parent = Q_NULLPTR);
	virtual ~VM(void);

public:
	void setTrapHandler(TrapHandler *p);

protected:
	void run(void) Q_DECL_OVERRIDE;

private:
	TrapHandler *m_trapHandler;
};

SGE_END

#endif // SGE_VM_HPP
