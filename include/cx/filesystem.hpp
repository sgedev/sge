//
//
#ifndef CX_FILESYSTEM_HPP
#define CX_FILESYSTEM_HPP

#include <string>

#include <cx/common.hpp>
#include <cx/noncopyable.hpp>

CX_BEGIN

class Filesystem: public Noncopyable {
public:
	enum {
		FLAG_READONLY = 0x1,
	};

	enum {
		OPEN_CREATE = 0x1,
		OPEN_READ = 0x2,
		OPEN_WRITE = 0x4
	};

	enum FileSeekMode {
		FILE_SEEK_SET = 0,
		FILE_SEEK_CUR,
		FILE_SEEK_END
	};

	struct Entry {
		enum {
			TYPE_UNKNOWN = 0,
			TYPE_DIR,
			TYPE_FILE
		} type;
		std::string name;
	};

	typedef struct DirStruct Dir;
	typedef struct FileStruct File;

public:
	Filesystem(void);
	virtual ~Filesystem(void);

public:
	virtual int flags(void) = 0;
	virtual Dir *openDir(const char *dirpath) = 0;
	virtual void close(Dir *dir) = 0;
	virtual bool readDir(Dir *dir, Entry *ent) = 0;
	virtual File *openFile(const char *filepath, int flags = OPEN_READ) = 0;
	virtual void close(File *fp) = 0;
	virtual int64_t getFileSize(File *fp) = 0;
	virtual int64_t seekFile(File *fp, int64_t offset, FileSeekMode fsm = FILE_SEEK_SET) = 0;
	virtual int64_t readFile(File *fp, void *buf, int64_t len) = 0;
	virtual int64_t writeFile(File *fp, const void *buf, int64_t len) = 0;
};

inline Filesystem::Filesystem(void)
{
}

inline Filesystem::~Filesystem(void)
{
}

class Dir {
public:
	Dir(Filesystem &fs, Filesystem::Dir *dir = NULL);
	virtual ~Dir(void);

private:
	Filesystem &m_fs;
	Filesystem::Dir *m_dir;
};

inline Dir::Dir(Filesystem &fs, Filesystem::Dir *dir)
	: m_fs(fs)
	, m_dir(dir)
{
}

inline Dir::~Dir(void)
{
	if (m_dir != NULL)
		m_fs.close(m_dir);
}

class File {
public:
	File(Filesystem &fs, Filesystem::File *file = NULL);
	virtual ~File(void);

private:
	Filesystem &m_fs;
	Filesystem::File *m_file;
};

inline File::File(Filesystem &fs, Filesystem::File *file)
	: m_fs(fs)
	, m_file(file)
{
}

inline File::~File(void)
{
	if (m_file != NULL)
		m_fs.close(m_file);
}

CX_API Filesystem *createNativeFilesystem(const char *root_path);

CX_END

#endif // CX_FILESYSTEM_HPP
