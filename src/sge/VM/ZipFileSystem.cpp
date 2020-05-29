//
//
#include <algorithm>

#include <SGE/VM/ZipFileSystem.hpp>

SGE_VM_BEGIN

// ZipDir

class ZipDir: public Dir {
public:
	ZipDir(ZipStatMap &dstat, const std::string &filter);
	virtual ~ZipDir(void);

public:
	void reset(void) override;
	bool next(Entry &ent) override;

private:
	ZipStatMap::iterator find(ZipStatMap::iterator it);

private:
	ZipStatMap &m_dstat;
	std::string m_filter;
	ZipStatMap::iterator m_curr;
};

ZipDir::ZipDir(ZipStatMap &dstat, const std::string &filter):
	m_dstat(dstat),
	m_filter(filter)
{
	reset();
}

ZipDir::~ZipDir(void)
{
}

void ZipDir::reset(void)
{
	m_curr = find(m_dstat.begin());
}

bool ZipDir::next(Entry &ent)
{
	if (m_curr == m_dstat.end())
		return false;

	ent.name = m_curr->first;
	if (m_curr->second.m_is_directory)
		ent.type = Entry::Type::Dir;
	else
		ent.type = Entry::Type::File;

	m_curr = find(m_curr);

	return true;
}

ZipStatMap::iterator ZipDir::find(ZipStatMap::iterator it)
{
	while (it != m_dstat.end()) {
		if (m_filter.rfind(m_curr->first, 0) == 0)
			return it;
		++it;
	}

	return m_dstat.end();
}

// ZipFile

class ZipFile : public File {
public:
	ZipFile(void);
	virtual ~ZipFile(void);

public:
	bool open(mz_zip_archive &zip, int index);
	int64_t size(void) const override;
	int64_t read(void *buf, int64_t size, int64_t offset) override;

private:
	int m_index;
	void *m_data;
	mz_uint64 m_size;
};

ZipFile::ZipFile(void):
	m_index(-1),
	m_data(NULL),
	m_size(0)
{
}

ZipFile::~ZipFile(void)
{
	if (m_data != NULL)
		free(m_data);
}

bool ZipFile::open(mz_zip_archive &zip, int index)
{
	SGE_ASSERT(m_index < 0);
	SGE_ASSERT(m_data == NULL);

	m_data = mz_zip_reader_extract_to_heap(&zip, index, &m_size, 0);
	if (m_data == NULL)
		return false;

	m_index = index;
	return true;
}

int64_t ZipFile::size(void) const
{
	SGE_ASSERT(m_index > 0);
	return m_size;
}

int64_t ZipFile::read(void *buf, int64_t size, int64_t offset)
{
	SGE_ASSERT(m_index > 0);
	SGE_ASSERT(offset >= 0);
	SGE_ASSERT(offset < m_size);
	
	int64_t len = m_size - offset;
	len = std::min(len, size);
	if (len > 0)
		memcpy(buf, ((unsigned char *)m_data) + offset, len);

	return len;
}

// ZipMountPoint

ZipFileSystem::ZipFileSystem()
{
	mz_zip_zero_struct(&m_zip);
}

ZipFileSystem::~ZipFileSystem(void)
{
	mz_zip_reader_end(&m_zip);
}

bool ZipFileSystem::doInit(const std::string &nativePath)
{
	mz_bool ret = mz_zip_reader_init_file(&m_zip, nativePath.c_str(), 0);
	if (!ret)
		return false;

	char entryName[256];
	mz_zip_archive_file_stat st;

	mz_uint n = mz_zip_reader_get_num_files(&m_zip);
	for (mz_uint i = 0; i < n; ++i) {
		mz_uint len = mz_zip_reader_get_filename(&m_zip, i, entryName, sizeof(entryName));
		if (len < 1)
			continue;
		ret = mz_zip_reader_file_stat(&m_zip, i, &st);
		if (!ret)
			continue;
		std::string path = entryName;
		if (st.m_is_directory) {
			path.pop_back();
			m_dstat[path] = st;
		} else
			m_fstat[path] = st;
	}

	return true;
}

Dir::Entry::Type ZipFileSystem::doType(const std::string &path)
{
	if (m_dstat.count(path.data() + 1) > 0)
		return Dir::Entry::Type::Dir;

	if (m_fstat.count(path.data() + 1) > 0)
		return Dir::Entry::Type::Dir;

	return Dir::Entry::Type::Invalid;
}

bool ZipFileSystem::doExists(const std::string &path)
{
	return (doType(path) != Dir::Entry::Type::Invalid);
}

DirPtr ZipFileSystem::doOpenDir(const std::string &path)
{
	if (m_dstat.count(path.data() + 1) < 1)
		return DirPtr();

	return DirPtr(new ZipDir(m_dstat, path.data() + 1));
}

FilePtr ZipFileSystem::doOpenFile(const std::string &path, File::Mode mode)
{
	if (m_fstat.count(path.data() + 1) < 1)
		return FilePtr();

	const mz_zip_archive_file_stat &st = m_fstat[path.data() + 1];

	ZipFile *fp(new ZipFile());
	if (!fp)
		return FilePtr();
	if (!fp->open(m_zip, st.m_file_index)) {
		delete fp;
		return FilePtr();
	}

	return FilePtr(fp);
}

SGE_VM_END
