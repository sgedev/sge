//
//
#ifndef SGE_ENTITY_HPP
#define SGE_ENTITY_HPP

#include <sge/common.hpp>
#include <sge/node.hpp>

SGE_BEGIN

class Entity: public Node {
	Q_OBJECT

public:
	Entity(QObject *parent = Q_NULLPTR);
	virtual ~Entity(void);
};

SGE_END

#endif // SGE_ENTITY_HPP

