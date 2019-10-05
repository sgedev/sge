//
//
#include <string>

#include <cx/filesystem.hpp>

CX_BEGIN

class nativeFilesystem: public Filesystem {
public:
	nativeFilesystem(const char *root);
	~nativeFilesystem(void);

public:
	int flags(void) override;
	Dir *openDir(const char *dirpath) override;
	void close(Dir *dir) override;
	bool readDir(Dir *dir, Entry *ent) override;
	File *openFile(const char *filepath, int flags) override;
	void close(File *fp) override;
	int64_t getFileSize(File *fp) override;
	int64_t seekFile(File *fp, int64_t offset, FileSeekMode fsm) override;
	int64_t readFile(File *fp, void *buf, int64_t len) override;
	int64_t writeFile(File *fp, const void *buf, int64_t len) override;

private:
	std::string m_root;
};

nativeFilesystem::nativeFilesystem(const char *root)
	: m_root(root)
{
}

nativeFilesystem::~nativeFilesystem(void)
{
}

int nativeFilesystem::flags(void)
{
	return 0;
}

Filesystem::Dir *nativeFilesystem::openDir(const char *dirpath)
{
	return NULL;
}

void nativeFilesystem::close(Dir *dir)
{

}

bool nativeFilesystem::readDir(Dir *dir, Entry *ent)
{
	return false;
}

Filesystem::File *nativeFilesystem::openFile(const char *filepath, int flags)
{
	return NULL;
}

void nativeFilesystem::close(File *fp)
{

}

int64_t nativeFilesystem::getFileSize(File *fp)
{
	return 0;
}

int64_t nativeFilesystem::seekFile(File *fp, int64_t offset, FileSeekMode fsm)
{
	return 0;
}

int64_t nativeFilesystem::readFile(File *fp, void *buf, int64_t len)
{
	return 0;
}

int64_t nativeFilesystem::writeFile(File *fp, const void *buf, int64_t len)
{
	return 0;
}

CX_API Filesystem *createNativeFilesystem(const char *root_path)
{
	return new nativeFilesystem(root_path);
}

CX_END
