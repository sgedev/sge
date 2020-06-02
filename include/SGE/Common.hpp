//
//
#ifndef SGE_COMMON_HPP
#define SGE_COMMON_HPP

#include <QtGlobal>
#include <QIODevice>
#include <QSharedPointer>

#include <SGE/Config.hpp>

#define SGE_INLINE inline

#define SGE_BEGIN namespace SGE {
#define SGE_END }

SGE_BEGIN

typedef QSharedPointer<QIODevice> IODevicePtr;

SGE_END

#endif // SGE_COMMON_HPP
