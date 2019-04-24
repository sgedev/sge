//
//
#include <cctype>
#include <cstring>

#include <sge/db.hpp>

SGE_DB_BEGIN

static mz_zip_archive s_archive;
static pugi::xml_document s_manifest;
static node s_root;

bool init(const char *filename)
{
	mz_bool zret;
	pugi::xml_parse_result xret;

	mz_zip_zero_struct(&s_archive);

	SGE_ASSERT(filename != NULL);

	zret = mz_zip_reader_init_file(&s_archive, filename, 0);
	if (!zret)
		return false;

	size_t size;
	void *p = mz_zip_reader_extract_file_to_heap(&s_archive, "manifest.xml", &size, 0);
	if (p == NULL) {
		mz_zip_reader_end(&s_archive);
		return false;
	}

	xret = s_manifest.load_buffer(p, size);
	free(p);

	if (!xret) {
		mz_zip_reader_end(&s_archive);
		return false;
	}

	pugi::xml_node n = s_manifest.child("SGE");
	if (!n) {
		mz_zip_reader_end(&s_archive);
		return false;
	}

	s_root = node(&s_archive, n);

	return true;
}

void shutdown(void)
{
	mz_zip_reader_end(&s_archive);
	s_root.set(NULL, pugi::xml_node());
}

node root(void)
{
	return s_root;
}

node get_node(const char *path)
{
	return s_root.child(path);
}

SGE_DB_END
