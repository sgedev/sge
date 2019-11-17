//
//
#include <QIODevice>
#include <QByteArray>
#include <QFileInfo>

#include <sge/launcher/zipfilesystem.hpp>

SGE_LAUNCHER_BEGIN

class ZipFile : public QIODevice {
public:
	ZipFile(mz_zip_archive &zip, int index);
	virtual ~ZipFile(void);

public:
	bool open(QIODevice::OpenMode mode) override;
	void close(void) override;
	qint64 pos(void) const override;
	bool seek(qint64 pos) override;
	qint64 size(void) const override;

protected:
	qint64 readData(char *data, qint64 maxSize) override;
	qint64 writeData(const char *data, qint64 maxSize) override;

private:
	mz_zip_archive &m_zip;
	int m_index;
	QByteArray m_data;
	qint64 m_pos;
};

ZipFile::ZipFile(mz_zip_archive &zip, int index)
	: m_zip(zip)
	, m_index(index)
	, m_pos(-1)
{
}

ZipFile::~ZipFile(void)
{
}

bool ZipFile::open(QIODevice::OpenMode mode)
{
	Q_ASSERT(!(mode & (QIODevice::WriteOnly | QIODevice::NewOnly | QIODevice::Truncate | QIODevice::Append)));
	Q_ASSERT(m_index >= 0);

	if (m_pos >= 0)
		return false;

	mz_bool ret;
	mz_zip_archive_file_stat st;

	ret = mz_zip_reader_file_stat(&m_zip, m_index, &st);
	if (!ret || st.m_is_directory)
		return false;

	m_data.resize(st.m_uncomp_size);
	ret = mz_zip_reader_extract_to_mem(&m_zip, m_index, m_data.data(), st.m_uncomp_size, 0);
	if (!ret)
		return false;

	if (!QIODevice::open(mode))
		return false;

	m_pos = 0;

	return true;
}

void ZipFile::close(void)
{
	QIODevice::close();
	m_data.clear();
	m_pos = -1;
}

qint64 ZipFile::pos(void) const
{
	return m_pos;
}

bool ZipFile::seek(qint64 pos)
{
	if (m_pos < 0)
		return false;
	if (pos < 0 || pos > m_data.size())
		return false;
	m_pos = pos;
	return true;
}

qint64 ZipFile::size(void) const
{
	Q_ASSERT(m_index >= 0);

	mz_zip_archive_file_stat st;
	mz_bool ret = mz_zip_reader_file_stat(&m_zip, m_index, &st);
	if (!ret || st.m_is_directory)
		return -1;
	return st.m_uncomp_size;
}

qint64 ZipFile::readData(char *data, qint64 maxSize)
{
	if (m_pos < 0)
		return -1;
	qint64 size = qMin(m_data.size() - m_pos, maxSize);
	if (size > 0) {
		memcpy(data, m_data.data() + m_pos, size);
		m_pos += size;
	}
	return size;
}

qint64 ZipFile::writeData(const char *data, qint64 maxSize)
{
	return -1;
}

ZipFileSystem::ZipFileSystem(QObject *parent)
	: FileSystem(parent)
{
	mz_zip_zero_struct(&m_zip);
}

ZipFileSystem::~ZipFileSystem(void)
{
	mz_zip_reader_end(&m_zip);
}

bool ZipFileSystem::load(const QString &filename)
{
	setReadonly(true);

	QFileInfo fi(filename);
	if (!fi.isReadable())
		return false;
	
	mz_bool ret = mz_zip_reader_init_file(&m_zip, filename.toStdString().c_str(), 0);
	if (!ret)
		return false;

	if (!doLoad())
		return false;

	return true;
}

bool ZipFileSystem::isDirectory(const QString &path)
{
	QString name(path.data() + 1);
	if (!name.endsWith('/'))
		name += '/';

	int index = mz_zip_reader_locate_file(&m_zip, name.toStdString().c_str(), NULL, 0);
	if (index < 0)
		return false;

	return mz_zip_reader_is_file_a_directory(&m_zip, index);
}

bool ZipFileSystem::isFile(const QString &path)
{
	int index = mz_zip_reader_locate_file(&m_zip, path.toStdString().c_str() + 1, NULL, 0);
	if (index < 0)
		return false;

	return !mz_zip_reader_is_file_a_directory(&m_zip, index);
}

qint64 ZipFileSystem::fileSize(const QString &filename)
{
	int index = mz_zip_reader_locate_file(&m_zip, filename.toStdString().c_str() + 1, NULL, 0);
	if (index < 0)
		return -1;

	mz_zip_archive_file_stat st;
	mz_bool ret = mz_zip_reader_file_stat(&m_zip, index, &st);
	if (!ret || st.m_is_directory)
		return -1;

	return st.m_uncomp_size;
}

FilePtr ZipFileSystem::openFile(const QString &filename, QIODevice::OpenMode mode)
{
	if (mode & (QIODevice::WriteOnly | QIODevice::NewOnly | QIODevice::Truncate | QIODevice::Append))
		return FilePtr();

	int index = mz_zip_reader_locate_file(&m_zip, filename.toStdString().c_str() + 1, NULL, 0);
	if (index < 0)
		return FilePtr();

	FilePtr file(new ZipFile(m_zip, index));
	if (!file || !file->open(mode))
		return FilePtr();

	return file;
}

bool ZipFileSystem::removeFile(const QString &filename)
{
	return false;
}

bool ZipFileSystem::makeDirectory(const QString &dirname)
{
	return false;
}

bool ZipFileSystem::removeDirectory(const QString &dirname)
{
	return false;
}

SGE_LAUNCHER_END
