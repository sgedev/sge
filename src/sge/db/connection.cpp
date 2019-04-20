//
//
#include <cctype>
#include <cstring>

#include <sge/db/connection.hpp>

SGE_DB_BEGIN

connection::connection(void)
{
	mz_zip_zero_struct(&m_archive);
}

connection::~connection(void)
{
	if (!m_filename.empty())
		close();
}

bool connection::open(const char *db_filename)
{
	mz_bool zret;
	pugi::xml_parse_result xret;

	SGE_ASSERT(db_filename != NULL);

	zret = mz_zip_reader_init_file(&m_archive, db_filename, 0);
	if (!zret)
		return false;

	size_t size;
	void *p = mz_zip_reader_extract_file_to_heap(&m_archive, "manifest.xml", &size, 0);
	if (p == NULL) {
		mz_zip_reader_end(&m_archive);
		return false;
	}

	xret = m_manifest.load_buffer(p, size);
	free(p);

	if (!xret) {
		mz_zip_reader_end(&m_archive);
		return false;
	}

	pugi::xml_node n = m_manifest.child("SGE");
	if (!n) {
		mz_zip_reader_end(&m_archive);
		return false;
	}

	m_root = node(&m_archive, n);
	m_filename = db_filename;

	return true;
}

void connection::close(void)
{
	mz_zip_reader_end(&m_archive);
	m_root.set(NULL, pugi::xml_node());
	m_filename.clear();
}

SGE_DB_END
