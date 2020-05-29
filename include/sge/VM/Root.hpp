//
//
#ifndef SGE_VM_ROOT_HPP
#define SGE_VM_ROOT_HPP

#include <map>
#include <list>
#include <memory>
#include <string>

#include <SGE/VM/Common.hpp>
#include <SGE/VM/NativeFileSystem.hpp>

SGE_VM_BEGIN

typedef std::shared_ptr<FileSystem> FileSystemPtr;

class Root: public NativeFileSystem {
public:
	Root(void);
	virtual ~Root(void);

public:
	bool mount(const std::string &path, const std::string &nativePath);
	bool umount(const std::string &path, const std::string &nativePath);

protected:
	bool doInit(const std::string &nativeRootPath) override;
	Dir::Entry::Type doType(const std::string &path) override;
	bool doExists(const std::string &path) override;
	DirPtr doOpenDir(const std::string &path) override;
	FilePtr doOpenFile(const std::string &path, File::Mode mode) override;

private:
	struct MountPoint {
		MountPoint(const std::string &p, FileSystemPtr mp):
			path(p), fs(mp) { }

		std::string path;
		FileSystemPtr fs;
	};

	typedef std::list<MountPoint> MountPointList;

	MountPointList::iterator findMountPoint(MountPointList::iterator it, const std::string &path, std::string &rpath);

	std::string m_nativeRootPath;
	MountPointList m_mountPointList;
};

SGE_VM_END

#endif // SGE_VM_ROOT_HPP
