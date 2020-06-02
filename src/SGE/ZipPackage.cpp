//
//
#include <SGE/ZipFile.hpp>
#include <SGE/ZipPackage.hpp>

SGE_BEGIN

ZipPackage::ZipPackage(const QString &nativePath):
	Package(nativePath)
{
}

ZipPackage::~ZipPackage(void)
{
}

bool ZipPackage::init(void)
{
	m_zip.reset(new ZipReader(nativePath()));
	if (!m_zip || !m_zip->init())
		return false;
	return true;
}

IODevicePtr ZipPackage::get(const QString &path)
{
	if (!m_zip)
		return IODevicePtr();

	int loc = m_zip->location(qPrintable(path) + 1);
	if (loc < 0)
		return IODevicePtr();

	return IODevicePtr(new ZipFile(m_zip, loc));
}

static bool build(const QString &dirname, const QString pkgname)
{
	return false;
}

SGE_END
