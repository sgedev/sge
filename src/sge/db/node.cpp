//
//
#include <string>

#include <sge/db/node.hpp>

SGE_DB_BEGIN

node node::child(const char *path)
{
	SGE_LOGI("path '%s'\n", path);

	SGE_ASSERT(check_path(path));

	std::string real_path = "/sge";
	real_path += path;

	pugi::xpath_node xnode = m_node.select_node(real_path.c_str());
	if (!xnode)
		return node();

	return node(xnode.node());
}

blob_ptr node::to_blob(void)
{
	blob_ptr p;

	const char *filename = to_string();
	if (filename == NULL)
		return p;

	// TODO use blob cache.

	int index = mz_zip_reader_locate_file(&internal::g_archive, filename + 1, NULL, 0);
	SGE_LOGD("index %d\n", index);
	if (index < 0) {
		SGE_LOGE("File '%s' not found in database.\n", filename);
		return p;
	}

	p.reset(new blob(filename + 1));

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
