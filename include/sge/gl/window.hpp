//
//
#ifndef SGE_GL_WINDOW_HPP
#define SGE_GL_WINDOW_HPP

#include <sge/noncopyable.hpp>
#include <sge/gl/common.hpp>

SGE_GL_BEGIN

class window: public noncopyable {
public:
	window(void);
	virtual ~window(void);

public:
	bool init(SDL_Window *from = NULL);
	void shutdown(void);
	bool handle_event(const SDL_Event &event);
	SDL_Window *to_sdl_window(void);
	Uint32 id(void) const;
	const glm::ivec2 &pos(void) const;
	const glm::ivec2 &size(void) const;
	const glm::ivec4 &rect(void) const;
	bool visibled(void) const;

private:
	SDL_Window *m_window;
	Uint32 m_id;
	glm::ivec2 m_pos;
	glm::ivec2 m_size;
	bool m_visibled;
	bool m_owned;
};

inline SDL_Window *window::to_sdl_window(void)
{
	SGE_ASSERT(m_window != NULL);

	return m_window;
}

inline Uint32 window::id(void) const
{
	SGE_ASSERT(m_window != NULL);

	return m_id;
}

inline const glm::ivec2 &window::pos(void) const
{
	SGE_ASSERT(m_window != NULL);

	return m_pos;
}

inline const glm::ivec2 &window::size(void) const
{
	SGE_ASSERT(m_window != NULL);

	return m_size;
}

inline const glm::ivec4 &window::rect(void) const
{
	SGE_ASSERT(m_window != NULL);

	return glm::ivec4(m_pos.x, m_pos.y, m_size.x, m_size.y);
}

inline bool window::visibled(void) const
{
	SGE_ASSERT(m_window != NULL);

	return m_visibled;
}

SGE_GL_END

#endif // SGE_GL_WINDOW_HPP
