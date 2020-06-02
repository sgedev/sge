//
//
#ifndef SGE_PACKAGE_HPP
#define SGE_PACKAGE_HPP

#include <QString>
#include <QIODevice>
#include <QSharedPointer>

#include <SGE/Common.hpp>

SGE_BEGIN

class Q_DECL_EXPORT Package {
public:
	Package(const QString &nativePath);
	virtual ~Package(void);

public:
	virtual bool init(void);
	virtual IODevicePtr get(const QString &path);
	const QString &nativePath(void) const;

private:
	QString m_nativePath;
};

SGE_INLINE const QString &Package::nativePath(void) const
{
	return m_nativePath;
}

SGE_END

#endif // SGE_PACKAGE_HPP
