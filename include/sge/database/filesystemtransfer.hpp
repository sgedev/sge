//
//
#ifndef SGE_DATABASE_FILESYSTEMTRANSFER_HPP
#define SGE_DATABASE_FILESYSTEMTRANSFER_HPP

#include <QString>
#include <QObject>

#include <sge/database/common.hpp>
#include <sge/database/filesystem.hpp>

SGE_DATABASE_BEGIN

class FileSystemTransfer: public QObject {
	Q_OBJECT

public:
	FileSystemTransfer(FileSystem *to, FileSystem *from, QObject *parent = Q_NULLPTR);

signals:
	void started(void);
	void ended(void);
	void currentArchiveIdChanged(const QString &id);

public:
	bool start(void);

private:
	FileSystem *m_to;
	FileSystem *m_from;
};

SGE_DATABASE_END

#endif // SGE_DATABASE_FILESYSTEMTRANSFER_HPP
