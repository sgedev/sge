//
//
#include <cctype>
#include <cstring>

#include <sge/fs/base.hpp>

SGE_FS_BEGIN

base::base(void)
{
}

base::~base(void)
{
}

bool base::check_path(const char *path)
{
	if (path == NULL)
		return false;

	if (*path != '/')
		return false;

	bool slashed = true;

	for (const char *p = path + 1; *p != '\0'; ++p) {
		if (isalnum(*p) || *p == '_') {
			slashed = false;
			continue;
		}
		if (*p != '/')
			return false;
		if (slashed)
			return false;
		slashed = true;
	}

	return true;
}

SGE_FS_END
