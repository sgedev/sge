//
//
#ifndef SGE_RESOURCE_MESH_HPP
#define SGE_RESOURCE_MESH_HPP

#include <QObject>

#include <sge/resource/common.hpp>

SGE_RESOURCE_BEGIN

class Mesh: public QObject {
	Q_OBJECT

public:
	Mesh(QObject *parent = Q_NULLPTR);
	virtual ~Mesh(void);
};

SGE_RESOURCE_END

#endif // SGE_RESOURCE_MESH_HPP

