//
//
#include <cstdio>
#include <filesystem>
#include <system_error>

#include <SGE/VM/NativeFileSystem.hpp>

SGE_VM_BEGIN

// NativeDir

class NativeDir: public Dir {
public:
	NativeDir(const std::string &nativePath);
	virtual ~NativeDir(void);

public:
	void reset(void) override;
	bool next(Entry &ent) override;

private:
	std::string m_nativePath;
	std::filesystem::directory_iterator m_curr;
};

NativeDir::NativeDir(const std::string &nativePath):
	m_nativePath(nativePath)
{
	reset();
}

NativeDir::~NativeDir(void)
{
}

void NativeDir::reset(void)
{
	m_curr = std::filesystem::directory_iterator(m_nativePath);
}

bool NativeDir::next(Entry &ent)
{
	std::filesystem::directory_iterator end;

	while (m_curr != end) {
		ent.name = m_curr->path().filename().string();
		if (m_curr->is_directory()) {
			ent.type = Entry::Type::Dir;
			return true;
		} else if (m_curr->is_regular_file()) {
			ent.type = Entry::Type::File;
			return true;
		}
		++m_curr;
	}

	return false;
}

// NativeFile

class NativeFile: public File {
public:
	NativeFile(void);
	virtual ~NativeFile(void);

public:
	bool open(const std::string &nativePath, Mode mode);
	int64_t size(void) const override;
	int64_t read(void *buf, int64_t size, int64_t offset) override;
	int64_t write(const void *buf, int64_t size, int64_t offset) override;

public:
	SDL_RWops *m_fp;
};

NativeFile::NativeFile(void):
	m_fp(NULL)
{
}

NativeFile::~NativeFile(void)
{
	if (m_fp != NULL)
		SDL_RWclose(m_fp);
}

bool NativeFile::open(const std::string &nativePath, Mode mode)
{
	std::string modeStr;
	switch (mode) {
	case ModeRead:
		modeStr = "r";
		break;
	case ModeWrite:
		modeStr = "w";
		break;
	case ModeAppend:
		modeStr = "a";
		break;
	case ModeRead | ModeWrite:
		modeStr = "r+";
		break;
	case ModeRead | ModeWrite | ModeCreate:
		modeStr = "w+";
		break;
	case ModeRead | ModeAppend:
		modeStr = "a+";
	}

	modeStr += 'b';

	m_fp = SDL_RWFromFile(nativePath.c_str(), modeStr.c_str());
	return (m_fp != NULL);
}

int64_t NativeFile::size(void) const
{
	SGE_ASSERT(m_fp != NULL);
	return SDL_RWsize(m_fp);
}

int64_t NativeFile::read(void *buf, int64_t size, int64_t offset)
{
	SGE_ASSERT(m_fp != NULL);

	if (SDL_RWseek(m_fp, offset, RW_SEEK_SET) < 0)
		return -1;

	return SDL_RWread(m_fp, buf, 1, size);
}

int64_t NativeFile::write(const void *buf, int64_t size, int64_t offset)
{
	SGE_ASSERT(m_fp != NULL);

	if (SDL_RWseek(m_fp, offset, RW_SEEK_SET) < 0)
		return -1;

	return SDL_RWwrite(m_fp, buf, 1, size);
}

// NativeMountPoint

NativeFileSystem::NativeFileSystem(void)
{
}

NativeFileSystem::~NativeFileSystem(void)
{
}

bool NativeFileSystem::doInit(const std::string &nativePath)
{
	return std::filesystem::is_directory(nativePath);
}

Dir::Entry::Type NativeFileSystem::doType(const std::string &path)
{
	std::string fullpath = nativePath() + path;
	if (std::filesystem::is_directory(fullpath))
		return Dir::Entry::Type::Dir;
	if (std::filesystem::is_regular_file(fullpath))
		return Dir::Entry::Type::File;
	return Dir::Entry::Type::Invalid;
}

bool NativeFileSystem::doExists(const std::string &path)
{
	return std::filesystem::exists(nativePath() + path);
}

DirPtr NativeFileSystem::doOpenDir(const std::string &path)
{
	std::string fullpath = nativePath() + path;
	if (!std::filesystem::is_directory(fullpath))
		return DirPtr();
	return DirPtr(new NativeDir(fullpath));
}

bool NativeFileSystem::doCreateDir(const std::string &path)
{
	std::error_code ec;
	std::filesystem::create_directories(nativePath() + path, ec);
	return !!ec;
}

FilePtr NativeFileSystem::doOpenFile(const std::string &path, File::Mode mode)
{
	auto fp(new NativeFile());
	if (!fp || !fp->open(nativePath() + path, mode)) {
		delete fp;
		return FilePtr();
	}
	return FilePtr(fp);
}

bool NativeFileSystem::doRemove(const std::string &path)
{
	std::error_code ec;
	std::filesystem::remove(nativePath() + path, ec);
	return !!ec;
}

SGE_VM_END
