//
//
#include <cctype>
#include <string>
#include <map>

#include <miniz_zip.h>
#include <pugixml.hpp>

#include <sge/db.hpp>

SGE_DB_BEGIN

enum type {
	TYPE_INVALID = 0,
	TYPE_BOOLEAN,
	TYPE_INTEGER,
	TYPE_NUMBER,
	TYPE_STRING,
	TYPE_VECTOR3I,
	TYPE_VECTOR3F,
	TYPE_BLOB
};

typedef std::map<std::string, blob_ptr> blob_cache_type;

static std::string filename;
static mz_zip_archive zip;
static pugi::xml_document manifest;
static pugi::xml_node root;
static int version;
static blob_cache_type blob_cache;

static type get_node_type(pugi::xml_node node)
{
	if (!node)
		return TYPE_INVALID;

	pugi::xml_attribute attr = node.attribute("TYPE");
	if (!attr)
		return TYPE_INVALID;

	const char *p = attr.value();

	if (strcmp(p, "BOOLEAN") == 0)
		return TYPE_BOOLEAN;

	if (strcmp(p, "INTEGER") == 0)
		return TYPE_INTEGER;

	if (strcmp(p, "NUMBER") == 0)
		return TYPE_NUMBER;

	if (strcmp(p, "STRING") == 0)
		return TYPE_STRING;

	if (strcmp(p, "VECTOR3I") == 0)
		return TYPE_VECTOR3F;

	if (strcmp(p, "VECTOR3F") == 0)
		return TYPE_VECTOR3F;

	if (strcmp(p, "BLOB") == 0)
		return TYPE_BLOB;

	return TYPE_INVALID;
}

static bool init_manifest(void)
{
	root = manifest.child("SGE");
	if (!root)
		return false;

	pugi::xml_attribute ver_attr = root.attribute("VERSION");
	if (!ver_attr)
		return false;

	int version = ver_attr.as_int();
	SGE_LOGI("manifest version %d\n", version);

	// TOOD check version.

	return true;
}

static bool check_path(const char *path)
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

bool init(const char *package_filename)
{
	SGE_ASSERT(package_filename != NULL);

	mz_zip_zero_struct(&zip);

	mz_bool zret = mz_zip_reader_init_file(&zip, package_filename, 0);
	if (!zret)
		return false;

	int index = mz_zip_reader_locate_file(&zip, "manifest.xml", NULL, 0);
	if (index < 0) {
		mz_zip_reader_end(&zip);
		return false;
	}

	size_t size;
	void *buffer = mz_zip_reader_extract_to_heap(&zip, index, &size, 0);
	if (buffer == NULL || size < 1) {
		mz_zip_reader_end(&zip);
		return false;
	}

	pugi::xml_parse_result xret = manifest.load_buffer(buffer, size);
	free(buffer);
	if (xret != pugi::status_ok) {
		mz_zip_reader_end(&zip);
		return false;
	}

	if (!init_manifest()) {
		mz_zip_reader_end(&zip);
		return false;
	}

	filename = package_filename;

	return true;
}

void shutdown(void)
{
	mz_zip_reader_end(&zip);
}

bool get_boolean(const char *path, bool def_value)
{
	SGE_ASSERT(check_path(path));

	pugi::xpath_node xnode = root.select_node(path + 1);
	if (!xnode)
		return def_value;

	if (get_node_type(xnode.node()) != TYPE_BOOLEAN)
		return def_value;

	return xnode.node().text().as_bool();
}

int get_integer(const char *path, int def_value)
{
	SGE_ASSERT(check_path(path));

	pugi::xpath_node xnode = root.select_node(path + 1);
	if (!xnode)
		return def_value;

	if (get_node_type(xnode.node()) != TYPE_INTEGER)
		return def_value;

	return xnode.node().text().as_int();
}

double get_number(const char *path, double def_value)
{
	SGE_ASSERT(check_path(path));

	pugi::xpath_node xnode = root.select_node(path + 1);
	if (!xnode)
		return def_value;

	if (get_node_type(xnode.node()) != TYPE_NUMBER)
		return def_value;

	return xnode.node().text().as_double();
}

const char *get_string(const char *path, const char *def_value)
{
	SGE_ASSERT(check_path(path));

	pugi::xpath_node xnode = root.select_node(path + 1);
	if (!xnode)
		return def_value;

	if (get_node_type(xnode.node()) != TYPE_STRING)
		return def_value;

	return xnode.node().value();
}

glm::ivec3 get_vector3i(const char *path, const glm::ivec3 def_value)
{
	SGE_ASSERT(check_path(path));

	pugi::xpath_node xnode = root.select_node(path + 1);
	if (!xnode)
		return def_value;

	pugi::xml_node node = xnode.node();

	if (get_node_type(node) != TYPE_VECTOR3I)
		return def_value;

	pugi::xml_node v1 = node.child("V1");
	pugi::xml_node v2 = node.child("V2");
	pugi::xml_node v3 = node.child("V3");

	return glm::ivec3(
		v1.text().as_int(),
		v2.text().as_int(),
		v3.text().as_int());
}

glm::vec3 get_vector3f(const char *path, const glm::vec3 def_value)
{
	SGE_ASSERT(check_path(path));

	pugi::xpath_node xnode = root.select_node(path + 1);
	if (!xnode)
		return def_value;

	pugi::xml_node node = xnode.node();

	if (get_node_type(node) != TYPE_VECTOR3F)
		return def_value;

	pugi::xml_node v1 = node.child("V1");
	pugi::xml_node v2 = node.child("V2");
	pugi::xml_node v3 = node.child("V3");

	return glm::vec3(
		v1.text().as_float(),
		v2.text().as_float(),
		v3.text().as_float());
}

blob_ptr get_blob(const char *path)
{
	blob_ptr p;

	SGE_ASSERT(check_path(path));

	pugi::xpath_node xnode = root.select_node(path + 1);
	if (!xnode)
		return p;

	pugi::xml_node node = xnode.node();

	if (get_node_type(node) != TYPE_BLOB)
		return p;

	const char *zpath = node.value();
	if (zpath == NULL)
		return p;

	if (blob_cache.count(zpath) == 0) {
		p.reset(new blob());
		if (!p || !p->init(&zip, zpath))
			return p;
		blob_cache[zpath] = p;
	} else
		p = blob_cache[zpath];

	return p;
}

SGE_DB_END

