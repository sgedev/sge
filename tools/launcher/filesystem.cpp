//
//
#include <physfs.h>

#include "filesystem.hpp"

class File :public sge::file {
public:
	File(void);
	virtual ~File(void);

protected:
	void __close(void) override;
	int64_t __size(void) override;
	int64_t __seek(int64_t offset, sge::file::seek_mode sm) override;
	int64_t __read(void* buf, int64_t len) override;
	int64_t __write(const void* buf, int64_t len) override;
};

File::File(void)
{
}

File::~File(void)
{
}

void File::__close(void)
{

}

int64_t File::__size(void)
{
	return 0;
}

int64_t File::__seek(int64_t offset, sge::file::seek_mode sm)
{
	return 0;
}

int64_t File::__read(void* buf, int64_t len)
{
	return 0;
}

int64_t File::__write(const void* buf, int64_t len)
{
	return 0;
}

class Dent :public sge::dent {
public:
	Dent(void);
	virtual ~Dent(void);

protected:
	void __close(void) override;
	bool __to_next(void) override;
};

Dent::Dent(void)
{

}

Dent::~Dent(void)
{

}

void Dent::__close(void)
{

}

bool Dent::__to_next(void)
{
	return false;
}

// Filesystem

Filesystem::Filesystem(const char* root)
{
}

Filesystem::~Filesystem(void)
{
}

const char* Filesystem::getcwd(void)
{
	return NULL;
}

sge::file* Filesystem::openfile(const char* path, int flags)
{
	return NULL;
}

sge::dent* Filesystem::opendir(const char* path)
{
	return NULL;
}
