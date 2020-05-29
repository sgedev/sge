//
//
#ifndef SGE_VM_FILESYSTEM_HPP
#define SGE_VM_FILESYSTEM_HPP

#include <memory>
#include <string>

#include <SGE/VM/Common.hpp>
#include <SGE/VM/Dir.hpp>
#include <SGE/VM/File.hpp>

SGE_VM_BEGIN

typedef std::shared_ptr<Dir> DirPtr;
typedef std::shared_ptr<File> FilePtr;

class FileSystem {
public:
	FileSystem(void);
	virtual ~FileSystem(void);

public:
	const std::string &nativePath(void) const;
	bool init(const std::string &nativePath);
	Dir::Entry::Type type(const std::string &nativePath);
	bool exists(const std::string &path);
	DirPtr openDir(const std::string &path);
	bool createDir(const std::string &path);
	FilePtr openFile(const std::string &path, File::Mode mode);
	bool remove(const std::string &path);
	static bool checkPath(const std::string &path);

protected:
	virtual bool doInit(const std::string &nativePath);
	virtual Dir::Entry::Type doType(const std::string &path);
	virtual bool doExists(const std::string &path);
	virtual DirPtr doOpenDir(const std::string &path);
	virtual bool doCreateDir(const std::string &path);
	virtual FilePtr doOpenFile(const std::string &path, File::Mode mode);
	virtual bool doRemove(const std::string &path);

private:
	std::string m_nativePath;
};

SGE_INLINE const std::string &FileSystem::nativePath(void) const
{
	return m_nativePath;
}

SGE_VM_END

#endif // SGE_VM_FILESYSTEM_HPP
