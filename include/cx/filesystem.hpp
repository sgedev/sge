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
	virtual const char *getcwd(void) = 0;
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

CX_END

#endif // CX_FILESYSTEM_HPP
