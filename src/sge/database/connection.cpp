//
//
#include <QByteArray>
#include <QStringList>
#include <QTextStream>

#include <sge/database/connection.hpp>

SGE_DATABASE_BEGIN

Connection::Connection(void)
	: m_fs(Q_NULLPTR)
{
}

bool Connection::reset(FileSystem *fs)
{
	Q_ASSERT(fs != Q_NULLPTR);

	if (fs->isReadonly())
		return false;

	FilePtr file(fs->openManifest(QIODevice::WriteOnly | QIODevice::Truncate));
	if (!file)
		return false;

	QTextStream stream(file.data());

	QDomDocument manifest;
	QDomElement root = manifest.createElement(Group::tag);
	if (root.isNull())
		return false;

	root.setAttribute(Group::attrName, "root");

	manifest.appendChild(root);
	manifest.save(stream, 2);

	QStringList list = fs->archiveList();
	for (auto it(list.begin()); it != list.end(); ++it)
		m_fs->removeArchive(*it);

	return load(fs);
}

bool Connection::load(FileSystem *fs)
{
	Q_ASSERT(fs != Q_NULLPTR);

	FilePtr manifestFile = fs->openManifest(QIODevice::ReadOnly);
	if (!manifestFile)
		return false;

	QByteArray d = manifestFile->readAll();
	qDebug() << d.toStdString().c_str();
	manifestFile->reset();

	if (!m_manifest.setContent(manifestFile.data()))
		return false;

	m_root = m_manifest.firstChildElement(Group::tag);
	if (m_root.isNull() || !m_root.hasAttribute(Group::attrName))
		return false;

	m_fs = fs;

	return true;
}

bool Connection::save(FileSystem *fs)
{
	Q_ASSERT(fs != Q_NULLPTR);

	if (fs->isReadonly())
		return false;

	FilePtr manifestFile = fs->openManifest(QIODevice::WriteOnly | QIODevice::Truncate);
	if (!manifestFile)
		return false;

	qDebug() << m_manifest.toString();

	QTextStream stream(manifestFile.data());
	m_manifest.save(stream, 2);

	return true;
}

SGE_DATABASE_END
