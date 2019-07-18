//
//
#ifndef SGE_SERVER_HPP
#define SGE_SERVER_HPP

#include <sge/common.hpp>
#include <sge/subsystem.hpp>

SGE_BEGIN

class server: public subsystem {
public:
	server(uv_loop_t *loop = NULL);
	virtual ~server(void);

protected:
	bool init(void) override;
	void shutdown(void) override;
};

SGE_END

#endif // SGE_SERVER_HPP

