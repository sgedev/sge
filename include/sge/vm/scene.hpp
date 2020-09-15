//
//
#ifndef SGE_VM_SCENE_HPP
#define SGE_VM_SCENE_HPP

#include <sge/vm/scene/common.hpp>
#include <sge/vm/scene/entity.hpp>

SGE_VM_SCENE_BEGIN

void init_exports(lua_State *L);
bool init(void);
void shutdown(void);
void update(float elapsed);

SGE_VM_SCENE_END

#endif // SGE_VM_SCENE_HPP
