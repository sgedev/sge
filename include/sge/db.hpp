//
//
#ifndef SGE_DB_HPP
#define SGE_DB_HPP

#include <memory>

#include <miniz_zip.h>
#include <pugixml.hpp>

#include <sge/common.hpp>

SGE_BEGIN

class db {
public:
	class blob {
	public:
		blob(const char *filename);
		virtual ~blob(void);

	public:
		bool open(void);
		void close(void);
		size_t get_size(void) const;
		const void *get_data(size_t offset = 0) const;
		size_t read(size_t offset, void *buf, size_t buf_size) const;

	private:
		std::string m_filename;
		void *m_data;
		size_t m_size;
	};

	typedef std::shared_ptr<blob> blob_ptr;

public:
	db(void);
	db(pugi::xml_node node);

public:
	const char *get_name(void) const;
	db parent(void);
	db first_child(void);
	db next_sibling(void);
	db select(const char *path);
	bool to_bool(void);
	int to_int(void);
	float to_float(void);
	double to_double(void);
	const char *to_string(void);
	bool is_blob(void) const;
	blob_ptr to_blob(void);
	static bool init(const char *filename);
	static void shutdown(void);
	static db root(void);

private:
	static bool init_manifest(void);
	bool check_path(const char *path);

private:
	pugi::xml_node m_node;
	static std::string c_filename;
	static mz_zip_archive c_archive;
	static pugi::xml_document c_manifest;
	static pugi::xml_node c_root;
	static int c_version;
};

SGE_END

#endif // SGE_DB_HPP

