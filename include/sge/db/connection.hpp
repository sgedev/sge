//
//
#ifndef SGE_DB_CONNECTION_HPP
#define SGE_DB_CONNECTION_HPP

#include <sge/noncopyable.hpp>
#include <sge/fs.hpp>
#include <sge/db/common.hpp>
#include <sge/db/node.hpp>

SGE_DB_BEGIN

class connection: public noncopyable {
public:
	connection(void);
	virtual ~connection(void);

public:
	bool init(fs::base *fs);
	void shutdown(void);
	node root(void);
	node get_node(const char *path);

private:
	bool load_manifest(fs::base *fs);

private:
	fs::base *m_fs;
	pugi::xml_document m_manifest;
	node m_root;
};

inline node connection::root(void)
{
	return m_root;
}

inline node connection::get_node(const char *path)
{
	return m_root.child(path);
}

SGE_DB_END

#endif // SGE_DB_CONNECTION_HPP
