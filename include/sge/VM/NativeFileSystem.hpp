//
//
#ifndef SGE_VM_NATIVEFILESYSTEM_HPP
#define SGE_VM_NATIVEFILESYSTEM_HPP

#include <memory>
#include <string>

#include <SGE/VM/Common.hpp>
#include <SGE/VM/FileSystem.hpp>

SGE_VM_BEGIN

typedef std::shared_ptr<Dir> DirPtr;
typedef std::shared_ptr<File> FilePtr;

class NativeFileSystem: public FileSystem {
public:
	NativeFileSystem(void);
	virtual ~NativeFileSystem(void);

protected:
	bool doInit(const std::string &nativePath) override;
	Dir::Entry::Type doType(const std::string &path) override;
	bool doExists(const std::string &path) override;
	DirPtr doOpenDir(const std::string &path) override;
	bool doCreateDir(const std::string &path) override;
	FilePtr doOpenFile(const std::string &path, File::Mode mode) override;
	bool doRemove(const std::string &path) override;
};

SGE_VM_END

#endif // SGE_VM_NATIVEFILESYSTEM_HPP
