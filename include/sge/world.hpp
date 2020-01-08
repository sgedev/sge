//
//
#ifndef SGE_WORLD_HPP
#define SGE_WORLD_HPP

#include <sge/common.hpp>
#include <sge/node.hpp>

SGE_BEGIN

class World: public Node {
	Q_OBJECT

public:
	World(QObject *parent = Q_NULLPTR);
	virtual ~World(void);
};

SGE_END

#endif // SGE_WORLD_HPP

