//
//
#include <sge/zip_reader.hpp>

SGE_BEGIN

zip_reader::zip_reader(void)
{
	mz_zip_zero_struct(&m_archive);
}

zip_reader::zip_reader(const std::string &filename) :
	m_filename(filename)
{
	mz_zip_zero_struct(&m_archive);
}

zip_reader::~zip_reader(void)
{
}

const std::string &zip_reader::filename(void) const
{
	return m_filename;
}

void zip_reader::set_filename(const std::string &r)
{
	SGE_ASSERT(!is_open());
	m_filename = r;
}

bool zip_reader::open(void)
{
	SGE_ASSERT(is_open());
	return mz_zip_reader_init_file(&m_archive, m_filename.c_str(), 0);
}

void zip_reader::close(void)
{
	SGE_ASSERT(is_open());
	mz_zip_reader_end(&m_archive);
}

bool zip_reader::is_open(void) const
{
	return (m_archive.m_zip_mode == MZ_ZIP_MODE_READING);
}

int zip_reader::count(void) const
{
	SGE_ASSERT(is_open());
	return mz_zip_reader_get_num_files(&m_archive);
}

int zip_reader::index(const std::string &path)
{
	SGE_ASSERT(is_open());
	return mz_zip_reader_locate_file(&m_archive, path.c_str(), nullptr, 0);
}

bool zip_reader::stat(int index, zip_reader::stat_t &r)
{
	SGE_ASSERT(is_open());
	return mz_zip_reader_file_stat(&m_archive, index, &r);
}

bool zip_reader::stat(const std::string &path, stat_t &r)
{
	SGE_ASSERT(is_open());
	return stat(index(path), r);
}

bool zip_reader::extract(int i, byte_array_t &ba)
{
	SGE_ASSERT(is_open());

	stat_t st;

	if (!stat(i, st) || st.m_is_directory)
		return false;

	ba.reserve(st.m_uncomp_size);

	return mz_zip_reader_extract_to_mem(&m_archive, i, ba.data(), st.m_uncomp_size, 0);
}

bool zip_reader::extract(const std::string &path, byte_array_t &ba)
{
	SGE_ASSERT(is_open());
	return extract(index(path), ba);
}

bool zip_reader::extract(int i, buffer &buf)
{
	SGE_ASSERT(is_open());
	return extract(i, buf.data);
}

bool zip_reader::extract(const std::string &path, buffer &buf)
{
	SGE_ASSERT(is_open());
	return extract(index(path), buf);
}

SGE_END
