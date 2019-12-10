//
//
#ifndef SGE_COMMON_HPP
#define SGE_COMMON_HPP

#include <QtGlobal>

#include <quickjs.h>

#include <sge/config.hpp>

#ifdef SGE_DEBUG
#	include <QDebug>
#endif

#define SGE_BEGIN namespace SGE {
#define SGE_END }

SGE_BEGIN

enum Mode {
	ModeNull = 0,
	ModeLauncher,
	ModeEditor,
	ModeServer
};

SGE_END

#endif // SGE_COMMON_HPP

