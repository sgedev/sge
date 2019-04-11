//
//
#ifndef SGE_GUI_SYSTEM_HPP
#define SGE_GUI_SYSTEM_HPP

#include <sge/gui/common.hpp>

SGE_GUI_BEGIN

class system {
public:
	system(void);
	virtual ~system(void);

public:
	bool init(void);
	void shutdown(void);
	void update(float elapsed);
	void draw(void);
};

SGE_GUI_END

#endif // SGE_GUI_SYSTEM_HPP

