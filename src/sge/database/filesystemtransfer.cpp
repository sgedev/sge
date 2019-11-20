//
//
#include <QByteArray>

#include <sge/database/filesystemtransfer.hpp>

SGE_DATABASE_BEGIN

FileSystemTransfer::FileSystemTransfer(FileSystem *to, FileSystem *from, QObject *parnet)
	: QObject(parent)
	, m_to(to)
	, m_from(from)
{
	Q_ASSERT(m_to != Q_NULLPTR);
	Q_ASSERT(m_from != Q_NULLPTR);
}

bool FileSystemTransfer::start(void)
{
	Q_ASSERT(m_from != Q_NULLPTR);
	Q_ASSERT(m_to != Q_NULLPTR);

	if (to->isReadonly())
		return false;

	QStringList fromList = from->archiveList();

	for (auto it(fromList.begin()); it != fromList.end(); ++it) {
		FilePtr fromFile = from->openArchive(*it, QIODevice::ReadOnly);
		if (!fromFile)
			return false;
		QString toFileId = to->createArchive();
		if (toFileId.isEmpty())
			return false;
		FilePtr toFile = to->openArchive(toFileId, QIODevice::WriteOnly);
		if (!toFile)
			return false;
		while (!fromFile->atEnd()) {
			QByteArray data = fromFile->read(512);
			if (data.size() < 1)
				return false;
			if (toFile->write(data) != data.size())
				return false;
		}
	}

	return true;
}

SGE_DATABASE_END
