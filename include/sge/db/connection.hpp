//
//
#ifndef SGE_DB_CONNECTION_HPP
#define SGE_DB_CONNECTION_HPP

#include <string>

#include <sge/db/common.hpp>
#include <sge/db/node.hpp>

SGE_DB_BEGIN

class connection: public node {
public:
	connection(void);
	virtual ~connection(void);

public:
	bool open(const char *db_filename);
	void close(void);
	node get_root(void);
	node get_node(const char *path);
	const char *get_filename(void) const;

private:
	std::string m_filename;
	mz_zip_archive m_archive;
	pugi::xml_document m_manifest;
	node m_root;
};

inline node connection::get_root(void)
{
	return m_root;
}

inline node connection::get_node(const char *path)
{
	return m_root.child(path);
}

inline const char *connection::get_filename(void) const
{
	return m_filename.c_str();
}

SGE_DB_END

#endif // SGE_DB_CONNECTION_HPP
