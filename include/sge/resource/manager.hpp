//
//
#ifndef SGE_RESOURCE_MANAGER_HPP
#define SGE_RESOURCE_MANAGER_HPP

#include <QObject>

#include <sge/resource/common.hpp>

SGE_RESOURCE_BEGIN

class Manager : public QObject {
	Q_OBJECT

public:
	Manager(QObject *parent = Q_NULLPTR);
	virtual ~Manager(void);
};

SGE_RESOURCE_END

#endif // SGE_RESOURCE_MANAGER_HPP

