//
//
#ifndef SGE_SCENE_BASE_HPP
#define SGE_SCENE_BASE_HPP

#include <memory>

#include <sge/noncopyable.hpp>
#include <sge/scene/common.hpp>
#include <sge/scene/camera.hpp>
#include <sge/scene/node.hpp>

SGE_SCENE_BEGIN

typedef std::shared_ptr<node> node_ptr;
typedef std::shared_ptr<camera> camera_ptr;

class base: public noncopyable {
public:
	base(void);
	virtual ~base(void);

public:
	virtual bool init(void) = 0;
	virtual void shutdown(void) = 0;
	virtual void update(float elapsed) = 0;
	virtual void draw(camera &cam) = 0;
	virtual void reset(void) = 0;
	virtual void add_node(node_ptr node);
	virtual void remove_node(node_ptr node);
	virtual void add_camera(camera_ptr cam);
	virtual void remove_camera(camera_ptr cam);
};

SGE_SCENE_END

#endif // SGE_SCENE_BASE_HPP
