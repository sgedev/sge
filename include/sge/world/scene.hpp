//
//
#ifndef SGE_WORLD_SCENE_HPP
#define SGE_WORLD_SCENE_HPP

#include <entt/entt.hpp>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <sge/vm/kernel.hpp>
#include <sge/world/common.hpp>

SGE_WORLD_BEGIN

class Scene: public vm::Kernel {
public:
	Scene(vm::Context& context);
	~Scene() override;

public:

protected:
	entt::registry& registry() noexcept {
		return registry_;
	}

	void initSyscalls(lua_State* L) override;
	void frame(Clock::duration elapsed) noexcept override;

private:
	entt::registry registry_;
	JPH::PhysicsSystem physics_system_;
};

SGE_WORLD_END

#endif // SGE_WORLD_SCENE_HPP
