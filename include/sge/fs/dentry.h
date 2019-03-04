//
//
#ifndef SGE_DENTRY_H
#define SGE_DENTRY_H

#include <string>

#include <sge/fs/common.h>

SGE_FS_BEGIN

class dentry {
public:
	enum type {
		TYPE_UNKNOWN = 0,
		TYPE_DIR,
		TYPE_FILE
	};

public:
	dentry(void);
	virtual ~dentry(void);

public:
	const char *path(void) const;

private:
	type m_type;
	std::string m_path;
};

inline const char *dentry::path(void) const
{
	return m_path.c_str();
}

SGE_FS_END

#endif // SGE_DENTRY_H

