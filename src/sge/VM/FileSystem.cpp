//
//
#include <cctype>
#include <filesystem>

#include <SGE/VM/FileSystem.hpp>

SGE_VM_BEGIN

FileSystem::FileSystem(void)
{
}

FileSystem::~FileSystem(void)
{
}

bool FileSystem::init(const std::string &nativePath)
{
	if (!doInit(nativePath))
		return false;

	m_nativePath = nativePath;

	return true;
}

Dir::Entry::Type FileSystem::type(const std::string &path)
{
	if (!checkPath(path))
		return Dir::Entry::Type::Invalid;

	return doType(path);
}

bool FileSystem::exists(const std::string &path)
{
	if (!checkPath(path))
		return false;

	return doExists(path);
}

DirPtr FileSystem::openDir(const std::string &path)
{
	if (!checkPath(path))
		return false;

	return doOpenDir(path);
}

bool FileSystem::createDir(const std::string &path)
{
	if (!checkPath(path))
		return false;

	return doCreateDir(path);
}

FilePtr FileSystem::openFile(const std::string &path, File::Mode mode)
{
	if (!checkPath(path))
		return false;

	// TODO check modes

	return doOpenFile(path, mode);
}

bool FileSystem::remove(const std::string &path)
{
	if (!checkPath(path))
		return false;

	return doRemove(path);
}

bool FileSystem::checkPath(const std::string &path)
{
	if (path.empty())
		return false;

	if (path[0] != '/')
		return false;

	bool slashed = true;
	for (auto i = 1; i < path.size(); ++i) {
		if (path[i] == '/') {
			if (slashed)
				return false;
			slashed = true;
			continue;
		}
		if (!isalnum(path[i]) && path[i] != '_' && path[i] != '-')
			return false;
		slashed = false;
	}

	return true;
}

bool FileSystem::doInit(const std::string &nativePath)
{
	return false;
}

Dir::Entry::Type FileSystem::doType(const std::string &path)
{
	return Dir::Entry::Type::Invalid;
}

bool FileSystem::doExists(const std::string &path)
{
	return false;
}

DirPtr FileSystem::doOpenDir(const std::string &path)
{
	return DirPtr();
}

bool FileSystem::doCreateDir(const std::string &path)
{
	return false;
}

FilePtr FileSystem::doOpenFile(const std::string &path, File::Mode mode)
{
	return FilePtr();
}

bool FileSystem::doRemove(const std::string &path)
{
	return false;
}

SGE_VM_END
