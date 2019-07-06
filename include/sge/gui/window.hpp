//
//
#ifndef SGE_GUI_WINDOW_HPP
#define SGE_GUI_WINDOW_HPP

#include <string>

#include <sge/gui/common.hpp>
#include <sge/gui/widget.hpp>

SGE_GUI_BEGIN

class window: public widget {
public:
	window(void);
	virtual ~window(void);

public:
	virtual void update(float elapsed);

private:
};

SGE_GUI_END

#endif // SGE_GUI_WINDOW_HPP
