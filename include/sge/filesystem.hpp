//
//
#ifndef SGE_FILESYSTEM_HPP
#define SGE_FILESYSTEM_HPP

#include <string>

#include <cx/noncopyable.hpp>

#include <sge/common.hpp>

SGE_BEGIN

// file

class file : public cx::noncopyable {
public:
	enum seek_mode {
		SEEK_MODE_SET = 0,
		SEEK_MODE_CUR,
		SEEK_MODE_END
	};

public:
	file(void);
	virtual ~file(void);

public:
	void close(void);
	int64_t size(void);
	int64_t seek(int64_t offset, seek_mode sm = SEEK_MODE_SET);
	int64_t read(void* buf, int64_t len);
	int64_t write(const void* buf, int64_t len);

protected:
	virtual void __close(void) = 0;
	virtual int64_t __size(void) = 0;
	virtual int64_t __seek(int64_t offset, seek_mode sm) = 0;
	virtual int64_t __read(void* buf, int64_t len) = 0;
	virtual int64_t __write(const void* buf, int64_t len) = 0;

private:
	bool m_open;
};

inline file::file(void)
	: m_open(false)
{
}

inline file::~file(void)
{
	if (m_open)
		close();
}

inline void file::close(void)
{
	SGE_ASSERT(m_open);

	__close();
	m_open = false;
}

inline int64_t file::size(void)
{
	SGE_ASSERT(m_open);

	return __size();
}

inline int64_t file::seek(int64_t offset, sge::file::seek_mode sm)
{
	SGE_ASSERT(m_open);

	return __seek(offset, sm);
}

inline int64_t file::read(void* buf, int64_t len)
{
	SGE_ASSERT(m_open);

	return __read(buf, len);
}

inline int64_t file::write(const void* buf, int64_t len)
{
	SGE_ASSERT(m_open);

	return __write(buf, len);
}

// dent

class dent : public cx::noncopyable {
public:
	enum {
		TYPE_DIR = 0,
		TYPE_FILE
	};

public:
	dent(void);
	virtual ~dent(void);

public:
	void close(void);
	bool to_next(void);
	int type(void) const;
	const char* name(void) const;
	const char* path(void) const;

protected:
	virtual void __close(void) = 0;
	virtual bool __to_next(void) = 0;

protected:
	int m_type;
	std::string m_name;
	std::string m_path;

private:
	bool m_open;
};

inline dent::dent(void)
{
}

inline dent::~dent(void)
{
	if (m_open)
		close();
}

inline void dent::close(void)
{
	SGE_ASSERT(m_open);

	__close();
	m_open = false;
}

inline int dent::type(void) const
{
	SGE_ASSERT(m_open);

	return m_type;
}

inline const char* dent::name(void) const
{
	SGE_ASSERT(m_open);

	return m_name.c_str();
}

inline bool dent::to_next(void)
{
	SGE_ASSERT(m_open);

	return __to_next();
}

// filesystem

class filesystem: public cx::noncopyable {
public:
	enum open_flag {
		OPEN_FLAG_CREATE = 0x1,
		OPEN_FLAG_READ = 0x2,
		OPEN_FLAG_WRITE = 0x4
	};

public:
	filesystem(void);
	virtual ~filesystem(void);

public:
	virtual const char* getcwd(void) = 0;
	virtual file *openfile(const char* filepath, int flags = OPEN_FLAG_READ) = 0;
	virtual dent *opendir(const char* dirpath) = 0;
};

inline filesystem::filesystem(void)
{
}

inline filesystem::~filesystem(void)
{
}

SGE_END

#endif // SGE_FILESYSTEM_HPP
