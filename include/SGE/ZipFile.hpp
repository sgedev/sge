//
//
#ifndef SGE_ZIPFILE_HPP
#define SGE_ZIPFILE_HPP

#include <QString>
#include <QIODevice>
#include <QByteArray>

#include <miniz.h>
#include <miniz_zip.h>

#include <SGE/Common.hpp>
#include <SGE/ZipReader.hpp>

SGE_BEGIN

class Q_DECL_EXPORT ZipFile: public QIODevice {
	Q_OBJECT

public:
	ZipFile(ZipReaderPtr zip, int loc);

public:
	bool atEnd(void) const Q_DECL_OVERRIDE;
	void close(void) Q_DECL_OVERRIDE;
	bool isSequential(void) const Q_DECL_OVERRIDE;
	bool open(OpenMode mode) Q_DECL_OVERRIDE;
	qint64 pos(void) const Q_DECL_OVERRIDE;
	bool reset(void) Q_DECL_OVERRIDE;
	bool seek(qint64 pos) Q_DECL_OVERRIDE;
	qint64 size(void) const Q_DECL_OVERRIDE;

protected:
	qint64 readData(char *data, qint64 maxlen) Q_DECL_OVERRIDE;
	qint64 writeData(const char *data, qint64 len) Q_DECL_OVERRIDE;

private:
	ZipReaderPtr m_zip;
	int m_loc;
	QByteArray m_data;
	qint64 m_pos;
};

SGE_END

#endif // SGE_ZIPFILE_HPP
