//
//
#include <sge/db/node.hpp>

SGE_DB_BEGIN

node node::child(const char *path)
{
	SGE_ASSERT(check_path(path));

	pugi::xpath_node xnode = m_node.select_node(path);
	if (!xnode)
		return node();

	return node(m_fs, xnode.node());
}

fs::file_ptr node::to_file(void)
{
	fs::file_ptr fp;

	const char *filename = to_string();
	if (filename == NULL)
		return fp;

	return m_fs->get_file(filename);
}

bool node::check_path(const char *path)
{
	return m_fs->check_path(path);
}

SGE_DB_END
