//
//
#include <cctype>
#include <string>
#include <map>

#include <miniz_zip.h>
#include <pugixml.hpp>

#include <sge/db.hpp>

SGE_BEGIN

std::string db::c_filename;
mz_zip_archive db::c_archive;
pugi::xml_document db::c_manifest;
pugi::xml_node db::c_root;
int db::c_version;

db::blob::blob(const char *filename)
	: m_filename(filename)
	, m_data(NULL)
	, m_size(0)
{
	SGE_ASSERT(filename != NULL);
}

db::blob::~blob(void)
{
	if (m_data != NULL)
		close();
}

bool db::blob::open(void)
{
	SGE_ASSERT(m_data == NULL);

	int index = mz_zip_reader_locate_file(&c_archive, m_filename.c_str(), NULL, 0);
	if (index < 0)
		return false;

	m_data = mz_zip_reader_extract_to_heap(&c_archive, index, &m_size, 0);
	if (m_data == NULL)
		return false;

	return true;
}

void db::blob::close(void)
{
	SGE_ASSERT(m_data != NULL);
	SGE_ASSERT(m_size >= 0);

	free(m_data);

	m_data = NULL;
	m_size = 0;
}

size_t db::blob::get_size(void) const
{
	SGE_ASSERT(m_data != NULL);

	return m_size;
}

const void *db::blob::get_data(size_t offset) const
{
	SGE_ASSERT(m_data != NULL);
	SGE_ASSERT(offset < m_size);

	return (((unsigned char *)m_data) + offset);
}

size_t db::blob::read(size_t offset, void *buf, size_t buf_size) const
{
	SGE_ASSERT(m_data != NULL);
	SGE_ASSERT(offset < m_size);

	size_t len = std::min(m_size - offset, buf_size);
	if (len > 0)
		memcpy(buf, ((unsigned char *)m_data) + offset, len);

	return len;
}

db::db(void)
{
}

db::db(pugi::xml_node node)
	: m_node(node)
{
}

bool db::is_blob(void) const
{
	pugi::xml_attribute attr = m_node.attribute("TYPE");
	if (!attr)
		return false;
	if (strcmp(attr.value(), "BLOB"))
		return false;
	return true;
}

const char *db::get_name(void) const
{
	return m_node.name();
}

db db::parent(void)
{
	return db(m_node.parent());
}

db db::first_child(void)
{
	return db(m_node.first_child());
}

db db::next_sibling(void)
{
	return db(m_node.next_sibling());
}

db db::select(const char *path)
{
	SGE_ASSERT(check_path(path));

	return db(m_node.select_node(path).node());
}

bool db::to_bool(void)
{
	return m_node.text().as_bool();
}

int db::to_int(void)
{
	return m_node.text().as_int();
}

float db::to_float(void)
{
	return m_node.text().as_float();
}

double db::to_double(void)
{
	return m_node.text().as_double();
}

const char *db::to_string(void)
{
	return m_node.value();
}

db::blob_ptr db::to_blob(void)
{
	if (!is_blob())
		return blob_ptr();

	return blob_ptr(new blob(m_node.value()));
}

bool db::init(const char *filename)
{
	SGE_ASSERT(filename != NULL);

	mz_zip_zero_struct(&c_archive);

	mz_bool zret = mz_zip_reader_init_file(&c_archive, filename, 0);
	if (!zret)
		return false;

	int index = mz_zip_reader_locate_file(&c_archive, "manifest.xml", NULL, 0);
	if (index < 0) {
		mz_zip_reader_end(&c_archive);
		return false;
	}

	size_t size;
	void *buffer = mz_zip_reader_extract_to_heap(&c_archive, index, &size, 0);
	if (buffer == NULL || size < 1) {
		mz_zip_reader_end(&c_archive);
		return false;
	}

	pugi::xml_parse_result xret = c_manifest.load_buffer(buffer, size);
	free(buffer);
	if (xret != pugi::status_ok) {
		mz_zip_reader_end(&c_archive);
		return false;
	}

	if (!init_manifest()) {
		mz_zip_reader_end(&c_archive);
		return false;
	}

	c_filename = filename;

	return true;
}

void db::shutdown(void)
{
	mz_zip_reader_end(&c_archive);
}

db db::root(void)
{
	return db(c_root);
}

bool db::init_manifest(void)
{
	pugi::xml_node node = c_manifest.child("SGE");
	if (!node)
		return false;

	pugi::xml_attribute ver_attr = node.attribute("VERSION");
	if (!ver_attr)
		return false;

	c_version = ver_attr.as_int();
	SGE_LOGI("manifest version %d\n", c_version);

	// TOOD check version.

	c_root = node;

	return true;
}

bool db::check_path(const char *path)
{
       if (path == NULL)
               return false;

       if (*path != '/')
               return false;

       bool slashed = true;

       for (const char *p = path + 1; *p != '\0'; ++p) {
               if (isalnum(*p) || *p == '_')
                       slashed = false;
               else if (*p != '/')
                       return false;
               else if (slashed)
                       return false;
               else
                       slashed = true;
       }

       return true;
}

SGE_END

