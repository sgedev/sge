//
//
#include <QByteArray>
#include <QStringList>
#include <QTextStream>

#include <sge/database/session.hpp>

SGE_DATABASE_BEGIN

Session::Session(void)
	: m_fs(Q_NULLPTR)
{
}

Session::~Session(void)
{
}

bool Session::init(FileSystem *fs)
{
	Q_ASSERT(fs != Q_NULLPTR);

	if ( fs->isReadonly())
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
		fs->removeArchive(*it);

	return load(fs);
}

bool Session::load(FileSystem *fs)
{
	Q_ASSERT(fs != Q_NULLPTR);

	FilePtr manifestFile = m_fs->openManifest(QIODevice::ReadOnly);
	if (!manifestFile)
		return false;

	if (!m_manifest.setContent(manifestFile.data()))
		return false;

	m_root = m_manifest.firstChildElement(Group::tag);
	if (m_root.isNull() || !m_root.hasAttribute(Group::attrName))
		return false;

	m_fs = fs;

	return true;
}

bool Session::save(FileSystem *fs)
{
	FileSystem *targetFS = fs ? fs : m_fs;

	if (targetFS->isReadonly())
		return false;

	FilePtr manifestFile = targetFS->openManifest(QIODevice::WriteOnly | QIODevice::Truncate);
	if (!manifestFile)
		return false;

	QTextStream stream(manifestFile.data());
	m_manifest.save(stream, 2);

	return true;
}

SGE_DATABASE_END
