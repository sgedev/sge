//
//
#ifndef SGE_CONSOLE_HPP
#define SGE_CONSOLE_HPP

#include <sge/common.hpp>

#define SGE_CONSOLE_BEGIN SGE_BEGIN namespace console {
#define SGE_CONSOLE_END } SGE_END

SGE_CONSOLE_BEGIN

bool init(void);
void shutdown(void);
void update(void);
void draw(void);
void enable(void);
void disable(void);

SGE_CONSOLE_END

#endif // SGE_CONSOLE_HPP

