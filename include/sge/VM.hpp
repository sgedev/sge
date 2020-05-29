//
//
#ifndef SGE_FILESYSTEM_HPP
#define SGE_FILESYSTEM_HPP

#include <map>
#include <string>

#include <SGE/Common.hpp>

SGE_BEGIN

class FileSystem {
public:
	FileSystem(void);
	virtual ~FileSystem(void);

public:
	bool init(const std::string &nativeRootPath);
	bool mount(const std::string &nativePath, const std::string &fs);
	bool umount(const std::string &path);

private:
	typedef std::map<std::string, std::string> MountPointMap;

	std::string m_nativeRootPath;
	MountPointMap m_mountPointMap;
};

SGE_END

#endif // SGE_FILESYSTEM_HPP
//
//
#ifndef SGE_VM_HPP
#define SGE_VM_HPP

#include <SGE/VM/Common.hpp>
#include <SGE/VM/Kernel.hpp>

#endif // SGE_VM_HPP
