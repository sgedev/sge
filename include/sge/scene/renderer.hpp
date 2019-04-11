//
//
#ifndef SGE_SCENE_RENDERER_HPP
#define SGE_SCENE_RENDERER_HPP


#include <sge/noncopyable.hpp>
#include <sge/gl.hpp>
#include <sge/scene/common.hpp>
#include <sge/scene/drawable.hpp>

SGE_SCENE_BEGIN

class renderer: public noncopyable {
public:
	renderer(void);
	virtual ~renderer(void);

public:
	bool init(void);
	void shutdown(void);
	bool begin(void);
	void end(void);
	void draw(drawable *p);

private:
	bool m_began;
};

SGE_SCENE_END

#endif // SGE_SCENE_RENDERER_HPP

