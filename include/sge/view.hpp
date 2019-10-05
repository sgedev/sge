//
//
#ifndef SGE_VIEW_HPP
#define SGE_VIEW_HPP

#include <glex.hpp>

#include <sge/common.hpp>

SGE_BEGIN

class View {
public:
	View(void);
	virtual ~View(void);

public:
	bool init(GLEX::Context *context);
	void shutdown(void);
	void render(void);

private:
	GLEX::Context *m_context;
};

SGE_END

#endif // SGE_VIEW_HPP

