//
//
#ifndef SGE_VM_ZIPFILESYSTEM_HPP
#define SGE_VM_ZIPFILESYSTEM_HPP

#include <map>
#include <string>

#include <miniz.h>
#include <miniz_zip.h>

#include <SGE/VM/Common.hpp>
#include <SGE/VM/FileSystem.hpp>

SGE_VM_BEGIN

typedef std::map<std::string, mz_zip_archive_file_stat> ZipStatMap;

class ZipFileSystem : public FileSystem {
public:
	ZipFileSystem(void);
	virtual ~ZipFileSystem(void);

protected:
	bool doInit(const std::string &nativePath) override;
	Dir::Entry::Type doType(const std::string &path) override;
	bool doExists(const std::string &path) override;
	DirPtr doOpenDir(const std::string &path) override;
	FilePtr doOpenFile(const std::string &path, File::Mode mode) override;

private:
	mz_zip_archive m_zip;
	ZipStatMap m_dstat;
	ZipStatMap m_fstat;
};

SGE_VM_END

#endif // SGE_VM_ZIPFILESYSTEM_HPP
