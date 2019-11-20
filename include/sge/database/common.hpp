//
//
#ifndef SGE_DATABASE_COMMON_HPP
#define SGE_DATABASE_COMMON_HPP

#include <QIODevice>
#include <QSharedPointer>

#include <sge/common.hpp>

#define SGE_DATABASE_BEGIN SGE_BEGIN namespace Database {
#define SGE_DATABASE_END } SGE_END

#define QIODEVICE_WRITE_MODES \
	(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Truncate | QIODevice::NewOnly)

SGE_DATABASE_BEGIN

typedef QSharedPointer<QIODevice> FilePtr;

SGE_DATABASE_END

#endif // SGE_DATABASE_COMMON_HPP
