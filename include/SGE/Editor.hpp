//
//
#ifndef SGE_EDITOR_HPP
#define SGE_EDITOR_HPP

#include <string>

#include <SGE/Player.hpp>

SGE_BEGIN

class Editor: public Player {
public:
	Editor(uv_loop_t *loop);
	virtual ~Editor(void);

public:
	bool start(void) override;
	void stop(void) override;

protected:
	void updateGui(void) override;

private:
};

SGE_END

#endif // SGE_EDITOR_HPP
