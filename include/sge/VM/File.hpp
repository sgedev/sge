//
//
#ifndef SGE_VM_FILE_HPP
#define SGE_VM_FILE_HPP

#include <string>

#include <SGE/VM/Common.hpp>

SGE_VM_BEGIN

class File {
public:
	enum Mode {
		ModeRead = 0x1,
		ModeWrite = 0x2,
		ModeCreate = 0x4,
		ModeAppend = 0x8
	};

public:
	File(void);
	virtual ~File(void);

public:
	Mode mode(void) const;
	virtual int64_t size(void) const = 0;
	virtual int64_t read(void *buf, int64_t size, int64_t offset = 0) = 0;
	virtual int64_t write(const void *buf, int64_t size, int64_t offset = 0);

protected:
	Mode m_mode;
};

SGE_INLINE File::Mode File::mode(void) const
{
	return m_mode;
}

SGE_VM_END

#endif // SGE_VM_FILE_HPP
