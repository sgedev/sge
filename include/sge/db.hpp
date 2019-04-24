//
//
#ifndef SGE_DB_HPP
#define SGE_DB_HPP

#include <sge/db/common.hpp>
#include <sge/db/node.hpp>

SGE_DB_BEGIN

bool init(const char *filename);
void shutdown(void);
node root(void);
node get_node(const char *path);

SGE_DB_END

#endif // SGE_DB_HPP
