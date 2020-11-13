//
//
#ifndef SGE_VM_SCENE_WORLD_HPP
#define SGE_VM_SCENE_WORLD_HPP

#include <vector>

#include <sge/vm/scene/common.hpp>
#include <sge/vm/scene/camera.hpp>
#include <sge/vm/scene/entity.hpp>

SGE_VM_SCENE_BEGIN

//typedef std::vector<entity *> entity_ptr_array_t;

class world { RTTR_ENABLE()
public:
    world(void);
    virtual ~world(void);

public:
    bool init(void);
    void shutdown(void);
    void update(float elapsed);
    void handle_event(const SDL_Event &evt);
    void test(int i);

private:
    //entity_ptr_array_t m_entities;
};

SGE_VM_SCENE_END

#endif // SGE_VM_SCENE_WORLD_HPP
