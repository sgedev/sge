//
//
#ifndef SGE_EDITOR_HPP
#define SGE_EDITOR_HPP

#include <sge/common.hpp>
#include <sge/game.hpp>

SGE_BEGIN

class editor: public game {
public:
	editor(uv_loop_t *loop, const filesystem::path &p);
	virtual ~editor(void);

protected:
	bool init(void) override;
	void shutdown(void) override;
	void handle_event(const SDL_Event &event) override;
	void update(float elapsed) override;
};

SGE_END

#endif // SGE_EDITOR_HPP

