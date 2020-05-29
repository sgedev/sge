//
//
#include <filesystem>

#include <SGE/VM/NativeFileSystem.hpp>
#include <SGE/VM/ZipFileSystem.hpp>
#include <SGE/VM/Root.hpp>

SGE_VM_BEGIN

Root::Root(void)
{
}

Root::~Root(void)
{
}

bool Root::mount(const std::string &path, const std::string &nativePath)
{
	if (!checkPath(path))
		return false;

	for (auto pm: m_mountPointList) {
		if (pm.fs->nativePath() == nativePath && pm.path == path)
			return false;
	}

	FileSystemPtr mp;

	if (std::filesystem::is_directory(nativePath))
		mp.reset(new NativeFileSystem());
	else
		mp.reset(new ZipFileSystem());

	if (!mp || !mp->init(nativePath))
		return false;

	std::string npath = path;
	if (npath.back() != '/')
		npath += '/';

	m_mountPointList.emplace_front(npath, mp);

	return true;
}

bool Root::umount(const std::string &path, const std::string &nativePath)
{
	if (!checkPath(path))
		return false;

	m_mountPointList.remove_if(
		[nativePath, path](const MountPoint &pm) {
			return (pm.path == path && pm.fs->nativePath() == nativePath);
		}
	);

	return true;
}

bool Root::doInit(const std::string &nativeRootPath)
{
	if (!NativeFileSystem::doInit(nativeRootPath))
		return false;

	return true;
}

Dir::Entry::Type Root::doType(const std::string &path)
{
	std::string rpath;
	MountPointList::iterator it(findMountPoint(m_mountPointList.begin(), path, rpath));
	while (it != m_mountPointList.end()) {
		Dir::Entry::Type type = it->fs->type(rpath);
		if (type != Dir::Entry::Type::Invalid)
			return type;
		it = findMountPoint(++it, path, rpath);
	}

	return NativeFileSystem::doType(path);
}

bool Root::doExists(const std::string &path)
{
	std::string rpath;
	MountPointList::iterator it(findMountPoint(m_mountPointList.begin(), path, rpath));
	while (it != m_mountPointList.end()) {
		if (it->fs->exists(rpath))
			return true;
		it = findMountPoint(++it, path, rpath);
	}

	return NativeFileSystem::doExists(path);
}

DirPtr Root::doOpenDir(const std::string &path)
{
	std::string rpath;
	MountPointList::iterator it(findMountPoint(m_mountPointList.begin(), path, rpath));
	while (it != m_mountPointList.end()) {
		DirPtr dp = it->fs->openDir(rpath);
		if (dp)
			return dp;
		it = findMountPoint(++it, path, rpath);
	}

	return NativeFileSystem::doOpenDir(path);
}

FilePtr Root::doOpenFile(const std::string &path, File::Mode mode)
{
	std::string rpath;
	MountPointList::iterator it(findMountPoint(m_mountPointList.begin(), path, rpath));
	while (it != m_mountPointList.end()) {
		FilePtr fp = it->fs->openFile(rpath, mode);
		if (fp)
			return fp;
		it = findMountPoint(++it, path, rpath);
	}

	return NativeFileSystem::doOpenFile(path, mode);
}

Root::MountPointList::iterator Root::findMountPoint(MountPointList::iterator it, const std::string &path, std::string &rpath)
{
	while (it != m_mountPointList.end()) {
		if (path.rfind(it->path, 0) == 0) {
			rpath = path.data() + (it->path.length() - 1);
			break;
		}
		++it;
	}

	return m_mountPointList.end();
}

SGE_VM_END
