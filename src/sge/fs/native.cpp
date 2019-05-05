//
//
#include <filesystem>

#include <sge/fs/native.hpp>

SGE_FS_BEGIN

native::native(void)
{
}

native::~native(void)
{
}

bool native::open(const char *native_path)
{
	SGE_ASSERT(native_path != NULL);

	if (!std::filesystem::is_directory(native_path))
		return false;

	m_native_path = native_path;

	return true;
}

void native::close(void)
{

}

file_ptr native::get_file(const char *path)
{
	SGE_ASSERT(check_path(path));

	file_ptr fp;

	std::string data_path = m_native_path + path;
	if (!std::filesystem::exists(data_path))
		return fp;

	fp.reset(new file(SDL_RWFromFile(data_path.c_str(), "wb")));

	return fp;
}

file_ptr native::new_file(const char *path, int64_t size)
{
	return NULL;
}

SGE_FS_END
