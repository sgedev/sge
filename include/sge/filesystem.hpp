//
//
#ifndef SGE_FILESYSTEM_HPP
#define SGE_FILESYSTEM_HPP

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QIODevice>
#include <QFileInfo>

#include <sge/common.hpp>
#include <sge/physfile.hpp>

SGE_BEGIN

typedef QSharedPointer<QIODevice> FilePtr;

class FileSystem: public QObject {
	Q_OBJECT

public:
	FileSystem(QObject *parent = Q_NULLPTR);
	virtual ~FileSystem(void);

public:
	bool create(const QString &nativeRootPath);
	bool load(const QString &nativeRootPath);
	bool exists(const QString &path);
	FilePtr open(const QString &path, QIODevice::OpenMode openMode = QIODevice::ReadWrite);
	bool mkpath(const QString &path);

protected:
	bool readDataId(const QString &treePath, QString &dataId);

private:
	QFileInfo m_nativeRootInfo;
	QString m_nativeRootPath;
	QString m_nativeTreePath;
	QString m_nativeDataPath;
	QString m_virtualRootPath;
	QString m_virtualTreePath;
	QString m_virtualDataPath;
};

SGE_END

#endif // SGE_FILESYSTEM_HPP
