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

	return node(m_archive, xnode.node());
}

blob_ptr node::to_blob(void)
{
	blob_ptr p;

	const char *filename = to_string();
	if (filename == NULL)
		return p;

	p.reset(new blob(m_archive, filename));

	return p;
}

bool node::check_path(const char *path)
{
	if (path == NULL)
		return false;

	if (*path != '/')
		return false;

	bool slashed = true;

	for (const char *p = path + 1; *p != '\0'; ++p) {
		if (isalnum(*p) || *p == '_') {
			slashed = false;
			continue;
		}
		if (*p != '/')
			return false;
		if (slashed)
			return false;
		slashed = true;
	}

	return true;
}

SGE_DB_END
