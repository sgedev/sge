//
//
#include <rttr/registration>

#include <sge/vm/scene/world.hpp>

SGE_VM_SCENE_BEGIN

RTTR_REGISTRATION
{
    rttr::registration::class_<world>("world").
        method("test", &world::test);
}

world::world(void)
{
}

world::~world(void)
{
}

bool world::init(void)
{
    return true;
}

void world::shutdown(void)
{
}

void world::update(float elapsed)
{
}

void world::handle_event(const SDL_Event &evt)
{
}

void world::test(int i)
{
    SGE_LOGI(">> i%d", i);
}

SGE_VM_SCENE_END
