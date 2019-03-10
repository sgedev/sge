//
//
#ifndef SGE_FS_FILE_H
#define SGE_FS_FILE_H

#include <sge/fs/common.h>

SGE_FS_BEGIN

class file {
public:
	enum type {
		TYPE_INVALID = 0,
		TYPE_BOOL,
		TYPE_INT,
		TYPE_INT_ARRAY,
		TYPE_FLOAT,
		TYPE_FLOAT_ARRAY,
		TYPE_STRING,
		TYPE_STRING_ARRAY
	};

public:
	file(type t = TYPE_INVALID);
	virtual ~file(void);

public:
	type get_type(void) const;

private:
	type m_type;
};

inline file::type file::get_type(void) const
{
	return m_type;
}

SGE_FS_END

#endif // SGE_FS_FILE_H
