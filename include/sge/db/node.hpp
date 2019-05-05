//
//
#ifndef SGE_DB_NODE_HPP
#define SGE_DB_NODE_HPP

#include <memory>

#include <pugixml.hpp>

#include <sge/fs.hpp>
#include <sge/db/common.hpp>

SGE_DB_BEGIN

class node {
public:
	node(void);
	node(fs::base *fs, pugi::xml_node node);
	node(const node &that);

public:
	void set(fs::base *fs, pugi::xml_node node);
	node child(const char *path);
	node first_child(void);
	node next_sibling(void);

public:
	bool to_bool(void);
	int to_int(void);
	float to_float(void);
	double to_double(void);
	const char *to_string(void);
	fs::file_ptr to_file(void);

public:
	operator bool(void);
	bool operator==(const node &that);
	bool operator!=(const node &that);
	node &operator=(const node &that);

protected:
	bool check_path(const char *path);

private:
	fs::base *m_fs;
	pugi::xml_node m_node;
};

inline node::node(void)
	: m_fs(NULL)
{
}

inline node::node(fs::base *fs, pugi::xml_node node)
	: m_fs(fs)
	, m_node(node)
{
}

inline node::node(const node &that)
	: m_fs(that.m_fs)
	, m_node(that.m_node)
{
}

inline void node::set(fs::base *fs, pugi::xml_node node)
{
	m_fs = fs;
	m_node = node;
}

inline node node::first_child(void)
{
	return node(m_fs, m_node.first_child());
}

inline node node::next_sibling(void)
{
	return node(m_fs, m_node.next_sibling());
}

inline bool node::to_bool(void)
{
	return m_node.text().as_bool();
}

inline int node::to_int(void)
{
	return m_node.text().as_int();
}

inline float node::to_float(void)
{
	return m_node.text().as_float();
}

inline double node::to_double(void)
{
	return m_node.text().as_double();
}

inline const char *node::to_string(void)
{
	return m_node.value();
}

inline node::operator bool(void)
{
	return !!m_node;
}

inline bool node::operator==(const node &that)
{
	return (m_fs == that.m_fs && m_node == that.m_node);
}

inline bool node::operator!=(const node &that)
{
	return !((*this) == that);
}

inline node &node::operator=(const node &that)
{
	m_fs = that.m_fs;
	m_node = that.m_node;

	return (*this);
}

SGE_DB_END

#endif // SGE_DB_NODE_HPP
