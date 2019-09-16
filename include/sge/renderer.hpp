//
//
#ifndef SGE_RENDERER_HPP
#define SGE_RENDERER_HPP

#include <string>

#include <imgui.h>
#include <GLEX/glex.h>

#include <sge/common.hpp>

SGE_BEGIN

class renderer {
public:
	renderer(void);
	virtual ~renderer(void);

public:
	bool init(void);
	void shutdown(void);
	bool handle_event(const SDL_Event *event);
	bool begin(void);
	void end(void);

private:
	enum {
		FLAG_VISIBLED = 0x1,
		FLAG_FULLSCREEN = 0x2,
	};

private:
	SDL_Window *m_window;
	glm::ivec4 m_rect;
	Uint32 m_id;
	SDL_GLContext m_gl;
	ImGuiContext *m_imgui;
	GLEXContext *m_glex;
	int m_flags;
	std::string m_title;
};

SGE_END

#endif // SGE_RENDERER_HPP

