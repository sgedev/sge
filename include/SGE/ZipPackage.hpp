//
//
#ifndef SGE_ZIPPACKAGE_HPP
#define SGE_ZIPPACKAGE_HPP

#include <QString>

#include <SGE/Common.hpp>
#include <SGE/ZipReader.hpp>
#include <SGE/Package.hpp>

SGE_BEGIN

struct ZipPackage: public Package {
public:
	ZipPackage(const QString &zipFilename);
	virtual ~ZipPackage(void);

public:
	bool init(void) Q_DECL_OVERRIDE;
	IODevicePtr get(const QString &path) Q_DECL_OVERRIDE;
	static bool build(const QString &dirname, const QString pkgname);

private:
	ZipReaderPtr m_zip;
};

SGE_END

#endif // SGE_ZIPPACKAGE_HPP
