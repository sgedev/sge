//
//
#ifndef SGE_SCENE_HPP
#define SGE_SCENE_HPP

#include <sge/common.hpp>
#include <sge/fs.hpp>
#include <sge/camera.hpp>

SGE_BEGIN

class scene {
public:
	scene(void);
	virtual ~scene(void);

public:
	bool init(fs &root, const char *path);
	void shutdown(void);
	void update(float elapsed);
	void draw(camera *target);
};

SGE_END

#endif // SGE_SCENE_HPP

