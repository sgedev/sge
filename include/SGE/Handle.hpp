//
//
#ifndef SGE_HANDLE_HPP
#define SGE_HANDLE_HPP

#include <string>

#include <SGE/Common.hpp>

SGE_BEGIN

class Handle {
public:
	Handle(uv_loop_t *loop);
	virtual ~Handle(void);

public:
	uv_loop_t *loop(void);
	const std::string &path(void) const;
	bool started(void) const;
	virtual bool start(const std::string &path);
	virtual void stop(void);
	virtual bool handleEvent(const SDL_Event &event);

private:
	uv_loop_t *m_loop;
	std::string m_path;
	bool m_started;
};

SGE_INLINE uv_loop_t *Handle::loop(void)
{
	return m_loop;
}

SGE_INLINE const std::string &Handle::path(void) const
{
	return m_path;
}

SGE_INLINE bool Handle::started(void) const
{
	return m_started;
}

SGE_END

#endif // SGE_HANDLE_HPP
