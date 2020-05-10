//
//
#ifndef SGL_OPENGLRENDERER_HPP
#define SGL_OPENGLRENDERER_HPP

#include <SGL/Common.hpp>
#include <SGL/Renderer.hpp>

SGL_BEGIN

class OpenGLRenderer : public Renderer {
public:
	OpenGLRenderer(void);
	virtual ~OpenGLRenderer(void);
};

SGL_END

#endif // SGL_OPENGLRENDERER_HPP
