//
//
#include <sge/db.hpp>
#include <sge/input.hpp>
#include <sge/scene.hpp>
#include <sge/game.hpp>

#define SGE_GAME_DB_PATH "/init"

SGE_GAME_BEGIN

static db::node s_node;
static vm s_init;

bool init(void)
{
	db::node node = db::get_node(SGE_GAME_DB_PATH);
	if (!node)
		return false;

	db::blob_ptr initrc = node.to_blob();
	if (!initrc)
		return false;

	if (!s_init.start(initrc))
		return false;

	s_node = node;

	return true;
}

void shutdown(void)
{
}

void update(float elapsed)
{
}

bool can_quit(void)
{
	return true;
}

SGE_GAME_END

