//
//
#ifndef SGE_VM_DIR_HPP
#define SGE_VM_DIR_HPP

#include <string>

#include <SGE/VM/Common.hpp>

SGE_VM_BEGIN

class Dir {
public:
	struct Entry {
		enum class Type {
			Invalid = 0,
			Dir,
			File
		} type;
		std::string name;
	};

public:
	Dir(void);
	virtual ~Dir(void);

public:
	virtual void reset(void) = 0;
	virtual bool next(Entry &ent) = 0;
};

SGE_VM_END

#endif // SGE_VM_DIR_HPP
