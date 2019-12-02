//
//
#include <QByteArray>
#include <QFileInfo>

#include <sge/filesystemzip.hpp>

SGE_BEGIN

// ZipFile

class ZipFile : public QFileDevice {
public:
	ZipFile(mz_zip_archive *zip, int index);
	virtual ~ZipFile(void);

public:
	void close(void) override;
	bool isSequential(void) const override;
	bool open(QIODevice::OpenMode mode) override;
	qint64 pos(void) const override;
	bool reset(void) override;
	bool seek(qint64 pos) override;
	qint64 size(void) const override;

protected:
	qint64 readData(char *data, qint64 maxSize) override;
	qint64 writeData(const char *data, qint64 maxSize) override;

private:
	mz_zip_archive *m_zip;
	int m_index;
	QByteArray m_data;
	qint64 m_pos;
};

ZipFile::ZipFile(mz_zip_archive *zip, int index)
	: m_zip(zip)
	, m_index(index)
	, m_pos(-1)
{
	Q_ASSERT(m_zip != Q_NULLPTR);
	Q_ASSERT(m_index >= 0);
}

ZipFile::~ZipFile(void)
{
	if (m_pos >= 0)
		close();
}

void ZipFile::close(void)
{
	if (m_pos < 0)
		return;

	m_data.clear();
	m_pos = -1;

	QIODevice::close();
}

bool ZipFile::isSequential(void) const
{
	return false;
}

bool ZipFile::open(QIODevice::OpenMode mode)
{
	if (mode & QIODEVICE_WRITE_MODES)
		return false;

	if (m_index < 0)
		return false;

	if (m_pos >= 0)
		close();

	mz_bool ret;
	mz_zip_archive_file_stat st;

	ret = mz_zip_reader_file_stat(m_zip, m_index, &st);
	if (!ret)
		return false;

	m_data.resize(st.m_uncomp_size);
	ret = mz_zip_reader_extract_to_mem(m_zip, m_index, m_data.data(), m_data.size(), 0);
	if (!ret)
		return false;

	if (!QIODevice::open(mode)) {
		m_data.clear();
		m_pos = -1;
		return false;
	}

	m_pos = 0;

	return true;
}

qint64 ZipFile::pos(void) const
{
	if (m_pos < 0)
		return -1;

	return m_pos;
}

bool ZipFile::reset(void)
{
	if (m_pos < 0)
		return false;

	m_pos = 0;

	return true;
}

bool ZipFile::seek(qint64 pos)
{
	if (m_pos < 0)
		return false;

	m_pos = pos;

	if (m_pos < 0)
		m_pos = 0;

	if (m_pos > m_data.size())
		m_pos = m_data.size();

	return true;
}

qint64 ZipFile::size(void) const
{
	if (m_index < 0)
		return -1;

	mz_zip_archive_file_stat st;
	mz_bool ret = mz_zip_reader_file_stat(m_zip, m_index, &st);
	if (!ret)
		return -1;

	return st.m_uncomp_size;
}

qint64 ZipFile::readData(char *data, qint64 maxSize)
{
	if (m_pos < 0)
		return -1;

	qint64 len = m_data.size() - m_pos;
	len = qMin(len, maxSize);
	if (len > 0) {
		memcpy(data, m_data.data() + m_pos, len);
		m_pos += len;
	}

	return len;
}

qint64 ZipFile::writeData(const char *data, qint64 maxSize)
{
	return -1;
}

// FileSystemZip

FileSystemZip::FileSystemZip(void)
{
	setReadonly(true);
	mz_zip_zero_struct(&m_zip);
}

FileSystemZip::~FileSystemZip(void)
{
	mz_zip_reader_end(&m_zip);
}

bool FileSystemZip::init(const QString &filename)
{
	QFileInfo fi(filename);
	if (!fi.isFile())
		return false;

	mz_bool ret = mz_zip_reader_init_file(&m_zip, filename.toStdString().c_str(), 0);
	if (!ret)
		return false;

	m_fileList.clear();

	mz_uint n = mz_zip_reader_get_num_files(&m_zip);
	for (mz_uint i = 0; i < n; ++i) {
		char path[512] = { 0 };
		mz_uint len = mz_zip_reader_get_filename(&m_zip, i, path, sizeof(path) - 1);
		if (len > 0) {
			m_fileList[QString("/") + path] = i;
		}
	}

	m_path = filename;

	return true;
}

bool FileSystemZip::isDir(const QString &path)
{
	if (path.isNull() || path.isEmpty())
		return false;

	if (path.at(0) != '/')
		return false;

	QString tmp(path);
	if (!tmp.endsWith('/'))
		tmp += '/';

	int index = m_fileList.value(path, -1);
	if (index < 0)
		return false;

	return mz_zip_reader_is_file_a_directory(&m_zip, index);
}

bool FileSystemZip::createDir(const QString &dirname)
{
	return false;
}

bool FileSystemZip::removeDir(const QString &dirname)
{
	return false;
}

QStringList FileSystemZip::readDir(const QString &dirname)
{
	QStringList list;

	auto begin = m_fileList.lowerBound(dirname);
	auto end = m_fileList.upperBound(dirname);

	while (begin != end) {
		QFileInfo fi(begin.key());
		list << fi.baseName();
		++begin;
	}

	return list;
}

bool FileSystemZip::isFile(const QString &path)
{
	return (m_fileList.value(path, -1) >= 0);
}

FilePtr FileSystemZip::openFile(const QString &filename, QIODevice::OpenMode mode)
{
	if (mode & QIODEVICE_WRITE_MODES)
		return FilePtr();

	int index = m_fileList.value(filename, -1);
	if (index < 0)
		return FilePtr();

	if (mz_zip_reader_is_file_a_directory(&m_zip, index))
		return FilePtr();

	FilePtr file(new ZipFile(&m_zip, index));
	if (!file || !file->open(mode))
		return FilePtr();

	return file;
}

bool FileSystemZip::removeFile(const QString &filename)
{
	return false;
}

SGE_END
