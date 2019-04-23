//
//
#ifndef SGE_GUI_VIEWPORT_HPP
#define SGE_GUI_VIEWPORT_HPP

#include <sge/scene/camera.hpp>
#include <sge/gui/common.hpp>
#include <sge/gui/window.hpp>

SGE_GUI_BEGIN

class viewport: public window {
public:
	viewport(void);
	virtual ~viewport(void);

public:
	virtual bool init(window *parent);
	virtual void shutdown(void);
	virtual void update(float elapsed);
	virtual void draw(void);

private:
	glm::ivec4 m_rect;
	scene::camera m_camera;
};

SGE_GUI_END

#endif // SGE_GUI_WINDOW_HPP

