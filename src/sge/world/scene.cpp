//
//
#include <sge/world/scene.hpp>

SGE_WORLD_BEGIN

Scene::Scene(vm::Context& context)
	: vm::Kernel(context) {
}

Scene::~Scene() {
}

void Scene::initSyscalls(lua_State* L) {
	vm::Kernel::initSyscalls(L);
}

void Scene::frame(Clock::duration elapsed) noexcept {
	vm::Kernel::frame(elapsed);

	auto delta = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 1000000.0f;
	//physics_system_.Update(delta, 1);
}

SGE_WORLD_END
