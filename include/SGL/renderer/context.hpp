//
//
#ifndef SGL_RENDERER_CONTEXT_HPP
#define SGL_RENDERER_CONTEXT_HPP

#include <SGL/renderer/common.hpp>

SGL_RENDERER_BEGIN

class Context {
public:
	Context(void);
	virtual ~Context(void);

public:
	bool init(void);
};

SGL_RENDERER_END

#endif // SGL_RENDERER_CONTEXT_HPP
