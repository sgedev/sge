//
//
#ifndef SGE_GAME_HPP
#define SGE_GAME_HPP

#include <filesystem/path.h>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl.h>

#include <sge/common.hpp>
#include <sge/window.hpp>
#include <sge/scene.hpp>
#include <sge/view.hpp>
#include <sge/renderer.hpp>
#include <sge/runtime.hpp>

SGE_BEGIN

class game {
public:
	game(uv_loop_t *loop, const filesystem::path &path);
	virtual ~game(void);

public:
	bool start(void);
	void stop(void);
	void feed_event(const SDL_Event &event);
	bool started(void) const;
	window &main_window(void);
	unsigned int fps(void) const;

protected:
	virtual bool init(void);
	virtual void shutdown(void);
	virtual void handle_event(const SDL_Event &event);
	virtual void update(float elapsed);

private:
	bool init_imgui(void);
	void shutdown_imgui(void);
	void show_loading(void);
	void show_ready(void);
	void show_fps(void);
	void show_hud(void);
	void frame(void);
	static void frame_cb(uv_timer_t *timer);
	void state(void);
	static void state_cb(uv_timer_t *timer);

private:
	enum {
		FLAG_STARTED = 0x1,
		FLAG_WINDOW_VISIBLED = 0x2,
		FLAG_WINDOW_FULLSCREEN = 0x4,
	};

	enum {
		STATE_IDLE = 0,
		STATE_LOADING,
		STATE_READY,
		STATE_PLAYING,
	};

private:
	uv_loop_t *m_loop;
	filesystem::path m_path;
	uv_timer_t m_frame_timer;
	uv_timer_t m_state_timer;
	int m_flags;
	int m_state;
	uint64_t m_last;
	unsigned int m_fps;
	unsigned int m_fps_count;
	window m_main_window;
	renderer m_renderer;
	view m_view;
	scene m_scene;
	ImGuiContext *m_imgui;
	runtime m_runtime;
	bool m_show_fps;
	bool m_show_hud;
};

inline bool game::started(void) const
{
	return m_flags & FLAG_STARTED;
}

inline window &game::main_window(void)
{
	return m_main_window;
}

inline unsigned int game::fps(void) const
{
	return m_fps;
}

SGE_END

#endif // SGE_GAME_HPP

