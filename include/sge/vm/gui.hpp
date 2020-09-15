//
//
#ifndef SGE_VM_GUI_HPP
#define SGE_VM_GUI_HPP

#include <sge/vm/gui/common.hpp>
#include <sge/vm/gui/window.hpp>
#include <sge/vm/gui/widget.hpp>

SGE_VM_GUI_BEGIN

void init_exports(lua_State *L);
bool init(void);
void shutdown(void);
void update(float elapsed);

SGE_VM_GUI_END

#endif // SGE_VM_GUI_HPP
