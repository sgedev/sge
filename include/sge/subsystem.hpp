//
//
#ifndef SGE_SUBSYSTEM_HPP
#define SGE_SUBSYSTEM_HPP

#include <sge/common.hpp>

SGE_BEGIN

class subsystem {
public:
	subsystem(uv_loop_t *loop = NULL);
	virtual ~subsystem(void);

public:
	bool start(void);
	void stop(void);
	bool feed_event(const SDL_Event *event);
	bool started(void) const;
	uv_loop_t *loop(void);

protected:
	virtual bool init(void);
	virtual void shutdown(void);
	virtual bool handle_event(const SDL_Event *event);

private:
	uv_loop_t *m_loop;
	bool m_started;
};

inline bool subsystem::started(void) const
{
	return m_started;
}

inline uv_loop_t *subsystem::loop(void)
{
	return m_loop;
}


SGE_END

#endif // SGE_SUBSYSTEM_HPP

