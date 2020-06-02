//
//
#ifndef SGE_ZIPREADER_HPP
#define SGE_ZIPREADER_HPP

#include <QString>
#include <QByteArray>
#include <QSharedPointer>

#include <miniz.h>
#include <miniz_zip.h>

#include <SGE/Common.hpp>

SGE_BEGIN

class Q_DECL_EXPORT ZipReader {
public:
	ZipReader(const QString &filename);
	virtual ~ZipReader(void);

public:
	mz_zip_archive *handle(void);
	const QString &filename(void) const;
	bool init(void);
	int location(const QString &path);
	bool extract(int loc, QByteArray &out);
	bool extract(const QString &path, QByteArray &out);

private:
	QString m_filename;
	mz_zip_archive m_zip;
};

SGE_INLINE ZipReader::ZipReader(const QString &filename):
	m_filename(filename)
{
	mz_zip_zero_struct(&m_zip);
}

SGE_INLINE ZipReader::~ZipReader(void)
{
	mz_zip_reader_end(&m_zip);
}

SGE_INLINE const QString &ZipReader::filename(void) const
{
	return m_filename;
}

SGE_INLINE bool ZipReader::init(void)
{
	return mz_zip_reader_init_file(&m_zip, qPrintable(m_filename), 0);
}

SGE_INLINE int ZipReader::location(const QString &path)
{
	return mz_zip_reader_locate_file(&m_zip, qPrintable(path), NULL, 0);
}

SGE_INLINE bool ZipReader::extract(const QString &path, QByteArray &out)
{
	return extract(location(path), out);
}

typedef QSharedPointer<ZipReader> ZipReaderPtr;

SGE_END

#endif // SGE_ZIPREADER_HPP
