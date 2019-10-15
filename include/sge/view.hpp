//
//
#ifndef SGE_VIEW_HPP
#define SGE_VIEW_HPP

#include <GL/glex.h>

#include <sge/common.hpp>

SGE_BEGIN

class View {
public:
	View(void);
	virtual ~View(void);

public:
	bool init(GL3WGetProcAddressProc proc);
	void shutdown(void);
	void beginFrame(void);
	void endFrame(void);
	GLEXContext *glex(void);
	const glm::ivec4 &viewport(void) const;
	void setViewport(int x, int y, int width, int height);

private:
	GLEXContext *m_glex;
	glm::ivec4 m_viewport;
};

inline GLEXContext *View::glex(void)
{
	return m_glex;
}

inline const glm::ivec4 &View::viewport(void) const
{
	return m_viewport;
}

inline void View::setViewport(int x, int y, int width, int height)
{
	m_viewport[0] = x;
	m_viewport[1] = y;
	m_viewport[2] = width;
	m_viewport[3] = height;
}

SGE_END

#endif // SGE_VIEW_HPP

