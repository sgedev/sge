//
//
#ifndef CX_READONLYFS_HPP
#define CX_READONLYFS_HPP

#include <string>

#include <miniz_zip.h>

#include <cx/noncopyable.hpp>

#include <cx/common.hpp>

CX_BEGIN

// ReadonlyFS

class ReadonlyFS: public Noncopyable {
public:
	enum SeekMode {
		SEEK_MODE_SET = 0,
		SEEK_MODE_CUR,
		SEEK_MODE_END
	};

	struct Dent {
		enum {
			TYPE_DIR = 0,
			TYPE_FILE
		} type;
		std::string name;
	};

	typedef struct DirHandle Dir;
	typedef struct FileHandle File;

public:
	ReadonlyFS(void);
	virtual ~ReadonlyFS(void);

public:
	virtual bool exists(const char *path) = 0;
	virtual Dir *openDir(const char *dirname) = 0;
	virtual void closeDir(Dir *dir) = 0;
	virtual bool readDir(Dir *dir, Dent *ent) = 0;
	virtual int64_t fileSize(const char *filename) = 0;
	virtual int64_t fileSize(File *file) = 0;
	virtual File *openFile(const char *filename) = 0;
	virtual void closeFile(File *file) = 0;
	virtual int64_t seekFile(File *file, int64_t offset, SeekMode sm = SEEK_MODE_SET) = 0;
	virtual int64_t readFile(File *file, void *buf, int64_t len) = 0;
};

// ReadonlyDir

class ReadonlyDir: public Noncopyable {
public:
	ReadonlyDir(ReadonlyFS *fs);
	virtual ~ReadonlyDir(void);

public:
	bool open(const char *dirname);
	void close(void);
	bool read(ReadonlyFS::Dent *ent);
	ReadonlyFS *fs(void);
	ReadonlyFS::Dir *handle(void);

private:
	ReadonlyFS *m_fs;
	ReadonlyFS::Dir *m_dir;
};

inline ReadonlyDir::ReadonlyDir(ReadonlyFS *fs)
	: m_fs(fs)
	, m_dir(NULL)
{
	CX_ASSERT(m_fs != NULL);
}

inline ReadonlyDir::~ReadonlyDir(void)
{
	if (m_dir != NULL)
		close();
}

inline bool ReadonlyDir::open(const char *dirname)
{
	CX_ASSERT(dirname != NULL);
	CX_ASSERT(m_fs != NULL);
	CX_ASSERT(m_dir == NULL);

	m_dir = m_fs->openDir(dirname);

	return (m_dir != NULL);
}

inline void ReadonlyDir::close(void)
{
	CX_ASSERT(m_fs != NULL);
	CX_ASSERT(m_dir != NULL);

	m_fs->closeDir(m_dir);
	m_dir = NULL;
}

inline bool ReadonlyDir::read(ReadonlyFS::Dent *ent)
{
	CX_ASSERT(m_fs != NULL);
	CX_ASSERT(m_dir != NULL);

	return m_fs->readDir(m_dir, ent);
}

// ReadonlyFile

class ReadonlyFile: public Noncopyable {
public:
	ReadonlyFile(ReadonlyFS *fs);
	virtual ~ResonlyFile(void);

public:
	bool open(const char *dirname);
	void close(void);
	int64_t size(void);
	int64_t seek(int offset, ReadonlyFS::SeekMode sm = ReadonlyFS::SEEK_MODE_SET);
	int64_t read(void *buf, int64_t len);
	ReadonlyFS *fs(void);
	ReadonlyFS::File *handle(void);
	
private:
	ReadonlyFS *m_fs;
	ReadonlyFS::File *m_file;
};

inline ReadonlyFile::ReadonlyFile(ReadonlyFS *fs)
	: m_fs(fs)
	, m_file(NULL)
{
	CX_ASSERT(m_fs != NULL);
}

inline ReadonlyFile::~ReadonlyFile(void)
{
	if (m_file != NULL)
		close();
}

inline bool ReadonlyFile::open(const char *filename)
{
	CX_ASSERT(filename != NULL);
	CX_ASSERT(m_fs != NULL);
	CX_ASSERT(m_file == NULL);

	m_file = m_fs->openFile(filename);

	return (m_file != NULL);
}

inline void ReadonlyFile::close(void)
{
	CX_ASSERT(m_fs != NULL);
	CX_ASSERT(m_file != NULL);

	m_fs->closeFile(m_file);
	m_file = NULL;
}

inline int64_t ReadonlyFile::size(void)
{
	CX_ASSERT(m_fs != NULL);
	CX_ASSERT(m_file != NULL);

	return m_fs->fileSize(m_file);
}

inline int64_t ReadonlyFile::seek(int64_t offset, ReadonlyFS::SeekMode sm)
{
	CX_ASSERT(m_fs != NULL);
	CX_ASSERT(m_file != NULL);

	return m_fs->seekFile(m_file, offset, sm);
}

inline int64_t ReadonlyFile::read(void *buf, int64_t len)
{
	CX_ASSERT(m_fs != NULL);
	CX_ASSERT(m_file != NULL);

	return m_fs->readFile(m_file, buf, len);
}

// ReadonlyNativeFS

class ReadonlyNativeFS: public ReadonlyFS {
public:
	ReadonlyNativeFS(void);
	virtual ~ReadonlyNativeFS(void);

public:
	bool exists(const char *path) override;
	Dir *openDir(const char *dirname) override;
	void closeDir(Dir *dir) override;
	bool readDir(Dir *dir, Dent *ent) override;
	int64_t fileSize(const char *filename) override;
	int64_t fileSize(File *file) override;
	File *openFile(const char *filename) override;
	void closeFile(File *file) override;
	int64_t seekFile(File *file, int64_t offset, SeekMode sm) override;
	int64_t readFile(File *file, void *buf, int64_t len) override;

private:
	std::string m_root;
};

// ReadonlyZipFS

class ReadonlyZipFS: public ReadonlyFS::IO {
public:
	ReadonlyZipFS(void);
	virtual ~ReadonlyZipFS(void);

public:
	bool exists(const char *path) override;
	Dir *openDir(const char *dirname) override;
	void closeDir(Dir *dir) override;
	bool readDir(Dir *dir, Dent *ent) override;
	int64_t fileSize(const char *filename) override;
	int64_t fileSize(File *file) override;
	File *openFile(const char *filename) override;
	void closeFile(File *file) override;
	int64_t seekFile(File *file, int64_t offset, SeekMode sm) override;
	int64_t readFile(File *file, void *buf, int64_t len) override;

private:
	mz_zip_archive m_zip;
};

SGE_END

#endif // SGE_READONLYFS_HPP

