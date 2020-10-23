//
//
#include <sge/vm/common.hpp>

SGE_VM_BEGIN

const char *lua_metatablename(const rttr::type &type)
{
    SGE_ASSERT(type);

    return type.get_name().data();
}

SGE_VM_END
