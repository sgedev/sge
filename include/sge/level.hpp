//
//
#ifndef SGE_LEVEL_HPP
#define SGE_LEVEL_HPP

#include <sge/common.hpp>
#include <sge/database.hpp>

SGE_BEGIN

class Level {
public:
	Level(void);
	virtual ~Level(void);

private:
	Database::Group m_group;
};

SGE_END

#endif // SGE_LEVEL_HPP
