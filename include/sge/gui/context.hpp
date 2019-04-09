//
//
#ifndef SGE_GUI_CONTEXT_HPP
#define SGE_GUI_CONTEXT_HPP

#include <sge/renderer.hpp>
#include <sge/gui/common.hpp>

SGE_GUI_BEGIN

class context {
public:
	context(void);
	virtual ~context(void);

public:
	bool init(void);
	void shutdown(void);
	void update(float elapsed);
	void draw(renderer::context &r);
};

SGE_GUI_END

#endif // SGE_GUI_CONTEXT_HPP

