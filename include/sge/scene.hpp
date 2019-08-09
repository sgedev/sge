//
//
#ifndef SGE_SCENE_HPP
#define SGE_SCENE_HPP

#include <filesystem/path.h>

#include <sge/common.hpp>
#include <sge/view.hpp>

SGE_BEGIN

class scene {
public:
	scene(void);
	virtual ~scene(void);

public:
	bool init(void);
	void shutdown(void);
	void update(float elapsed);
	void draw(view &v);
};

SGE_END

#endif // SGE_SCENE_HPP

