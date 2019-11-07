//
//
#ifndef SGE_PHYSFILE_HPP
#define SGE_PHYSFILE_HPP

#include <QIODevice>

#include <physfs.h>

#include <sge/common.hpp>

SGE_BEGIN

class PhysFile : public QIODevice {
	Q_OBJECT

public:
	PhysFile(const QString &path, QObject *parent = Q_NULLPTR);
	virtual ~PhysFile(void);

public:
	bool atEnd(void) const override;
	bool open(QIODevice::OpenMode mode) override;
	void close(void) override;
	bool seek(qint64 pos) override;
	qint64 size(void) const override;
	bool isSequential(void) const override;

protected:
	qint64 readData(char *data, qint64 maxSize) override;
	qint64 writeData(const char *data, qint64 maxSize) override;

private:
	QString m_path;
	PHYSFS_File *m_file;
};

inline bool PhysFile::isSequential(void) const
{
	return false;
}

inline qint64 PhysFile::writeData(const char *data, qint64 maxSize)
{
	setErrorString("Write is unsupported for PFile object.");
	return -1;
}

SGE_END

#endif // SGE_PHYSFILE_HPP
