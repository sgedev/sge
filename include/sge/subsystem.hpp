//
//
#ifndef SGE_SUBSYSTEM_HPP
#define SGE_SUBSYSTEM_HPP

#include <string>

#include <filesystem/path.h>

#include <sge/common.hpp>

SGE_BEGIN

class subsystem {
public:
	subsystem(uv_loop_t *loop = NULL);
	virtual ~subsystem(void);

public:
	bool start(const filesystem::path &p);
	void stop(void);
	void feed_event(const SDL_Event &event);
	uv_loop_t *loop(void);
	bool started(void) const;

protected:
	const filesystem::path &path(void) const;
	virtual bool init(void) { return true; }
	virtual void shutdown(void) { }
	virtual void handle_event(const SDL_Event &event) { }

private:
	uv_loop_t *m_loop;
	bool m_started;
	filesystem::path m_path;
};

inline uv_loop_t *subsystem::loop(void)
{
	SGE_ASSERT(m_loop != NULL);
	return m_loop;
}

inline bool subsystem::started(void) const
{
	SGE_ASSERT(m_loop != NULL);
	return m_started;
}

inline const filesystem::path &subsystem::path(void) const
{
	return m_path;
}

SGE_END

#endif // SGE_SUBSYSTEM_HPP

