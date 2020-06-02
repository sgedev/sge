//
//
#include <QFileInfo>

#include <SGE/Package.hpp>

SGE_BEGIN

Package::Package(const QString &nativePath):
	m_nativePath(nativePath)
{
}

Package::~Package(void)
{
}

bool Package::init(void)
{
	QFileInfo info(m_nativePath);
	return info.isDir();
}

IODevicePtr Package::get(const QString &path)
{
	QString fullPath = m_nativePath + path;
	QFileInfo info(fullPath);
	if (!info.isFile())
		return IODevicePtr();
	return IODevicePtr(new QFile(fullPath));
}

SGE_END
