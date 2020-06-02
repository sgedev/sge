//
//
#include <SGE/ZipReader.hpp>

SGE_BEGIN

bool ZipReader::extract(int loc, QByteArray &out)
{
	if (loc < 0)
		return false;

	mz_bool ret;
	mz_zip_archive_file_stat st;

	ret = mz_zip_reader_file_stat(&m_zip, loc, &st);
	if (!ret)
		return false;

	out.resize(st.m_uncomp_size);

	if (st.m_uncomp_size > 0)
		return mz_zip_reader_extract_to_mem(&m_zip, loc, out.data(), st.m_uncomp_size, 0);

	return true;
}

SGE_END
