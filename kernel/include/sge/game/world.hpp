//
//
#ifndef SGE_SCENE_WORLD_HPP
#define SGE_SCENE_WORLD_HPP

#include <vector>

#include <sge/scene/common.hpp>
#include <sge/scene/camera.hpp>
#include <sge/scene/entity.hpp>

SGE_SCENE_BEGIN

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

SGE_SCENE_END

#endif // SGE_SCENE_WORLD_HPP
