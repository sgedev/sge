//
//
#ifndef SGE_GRAPHICS_CANVAS_HPP
#define SGE_GRAPHICS_CANVAS_HPP

#include <sge/graphics/common.hpp>

SGE_GRAPHICS_BEGIN

class canvas {
public:
	canvas(void);
	virtual ~canvas(void);

public:
	const glm::ivec2 &size(void) const;
	int width(void) const;
	int height(void) const;
	void resize(const glm::ivec2 &r);
	virtual void resize(int width, int height);
	virtual bool begin(void);
	virtual void end(void);

private:
	glm::ivec2 m_size;
};

SGE_INLINE const glm::ivec2 &canvas::size(void) const
{
	return m_size;
}

SGE_INLINE int canvas::width(void) const
{
	return m_size.x;
}

SGE_INLINE int canvas::height(void) const
{
	return m_size.y;
}

SGE_INLINE void canvas::resize(const glm::ivec2 &r)
{
	resize(r.x, r.y);
}

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_CANVAS_HPP
