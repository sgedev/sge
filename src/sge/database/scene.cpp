//
//
#include <sge/database/blob.hpp>

SGE_DATABASE_BEGIN

const QString Blob::tag("blob");
const QString Blob::attrName("name");

Blob Blob::next(void) const
{
	QDomElement element = m_element.nextSiblingElement(tag);
	while (!element.isNull()) {
		if (!element.attribute(attrName).isEmpty())
			return Blob(m_fs, element);
		element = element.nextSiblingElement(tag);
	}

	return Blob();
}

FilePtr Blob::open(QIODevice::OpenMode mode)
{
	if (mode & (QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Append | QIODevice::NewOnly)) {
		if (m_fs->isReadonly())
			return FilePtr();
	}

	return m_fs->openArchive(archiveId(), mode);
}

SGE_DATABASE_END
