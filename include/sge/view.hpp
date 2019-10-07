//
//
#ifndef SGE_VIEW_HPP
#define SGE_VIEW_HPP

#include <glex.hpp>

#include <nanovg.h>
#include <nanovg_gl.h>
#include <nanovg_gl_utils.h>

#include <sge/common.hpp>

SGE_BEGIN

class View {
public:
	View(void);
	virtual ~View(void);

public:
	bool init(void);
	void shutdown(void);
	void beginFrame(void);
	void endFrame(void);
	GLEX::Context *glex(void);
	struct NVGcontext *nanovg(void);
	const glm::ivec4 &viewport(void) const;
	void setViewport(int x, int y, int width, int height);

private:
	GLEX::Context *m_glex;
	struct NVGcontext *m_nanovg;
	glm::ivec4 m_viewport;
};

inline GLEX::Context *View::glex(void)
{
	return m_glex;
}

inline struct NVGcontext *View::nanovg(void)
{
	return m_nanovg;
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

