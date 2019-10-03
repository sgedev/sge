//
//
#ifndef SGE_LAUNCHER_FILESYSTEM_HPP
#define SGE_LAUNCHER_FILESYSTEM_HPP

#include <string>

#include <sge/filesystem.hpp>

class Filesystem :public sge::filesystem {
public:
	Filesystem(const char* root);
	virtual ~Filesystem(void);

public:
	const char* getcwd(void) override;
	sge::file* openfile(const char* path, int flags) override;
	sge::dent* opendir(const char* path) override;

private:
	std::string m_root;
};

#endif // SGE_LAUNCHER_FILESYSTEM_HPP
