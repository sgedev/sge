//
//
#include <sge/db/connection.hpp>

SGE_DB_BEGIN

connection::connection(void)
	: m_fs(NULL)
{
}

connection::~connection(void)
{
	if (m_fs != NULL)
		shutdown();
}

bool connection::init(fs::base *fs)
{
	SGE_ASSERT(m_fs == NULL);

	m_manifest.reset();

	if (!load_manifest(fs))
		return false;

	pugi::xml_node root = m_manifest.child("SGE");
	if (!root) {
		m_manifest.reset();
		return false;
	}

	m_fs = fs;
	m_root = node(m_fs, root);

	return true;
}

void connection::shutdown(void)
{
	SGE_ASSERT(m_fs != NULL);

	m_manifest.reset();
	m_fs = NULL;
}

bool connection::load_manifest(fs::base *fs)
{
	SGE_ASSERT(fs != NULL);

	fs::file_ptr manifest_file = fs->get_file("/manifest.xml");
	if (!manifest_file)
		return false;

	int64_t size = manifest_file->size();
	if (size < 1)
		return false;

	void *p = malloc(size);
	if (p == NULL)
		return false;

	int64_t ret = manifest_file->read(p, size);
	if (ret != size) {
		free(p);
		return false;
	}

	pugi::xml_parse_result res = m_manifest.load_buffer(p, size);
	free(p);

	if (!res)
		return false;

	return true;
}

SGE_DB_END
