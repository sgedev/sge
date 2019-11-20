//
//
#include <sge/database/asset.hpp>

SGE_DATABASE_BEGIN

const QString Asset::tag("asset");
const QString Asset::attrName("name");

Asset Asset::nextAsset(void) const
{
	QDomElement element = m_element.firstChildElement(tag);
	while (!element.isNull()) {
		if (element.hasAttribute(attrName))
			return Asset(m_fs, element);
		element = element.nextSiblingElement(tag);
	}

	return Asset();
}

FilePtr Asset::open(QIODevice::OpenMode mode)
{
	if (mode & (QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Append | QIODevice::NewOnly)) {
		if (m_fs->isReadonly())
			return FilePtr();
	}

	return m_fs->openArchive(archiveId(), mode);
}

SGE_DATABASE_END
