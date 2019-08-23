//
//
#include <sge/game.hpp>

SGE_BEGIN

void game::init_traps(void)
{
	lua_newtable(m_L);

	lua_pushcfunction(m_L, &game::trap_version_fe);
	lua_setfield(m_L, -2, "version");

	lua_pushcfunction(m_L, &game::trap_task_fe);
	lua_setfield(m_L, -2, "task");

	lua_pushcfunction(m_L, &game::trap_current_fe);
	lua_setfield(m_L, -2, "current");

	lua_pushcfunction(m_L, &game::trap_sleep_fe);
	lua_setfield(m_L, -2, "sleep");

	lua_pushcfunction(m_L, &game::trap_fps_fe);
	lua_setfield(m_L, -2, "fps");

	lua_newtable(m_L);

	lua_pushcfunction(m_L, &game::trap_window_visibled_fe);
	lua_setfield(m_L, -2, "visibled");

	lua_pushcfunction(m_L, &game::trap_window_show_fe);
	lua_setfield(m_L, -2, "show");

	lua_pushcfunction(m_L, &game::trap_window_hide_fe);
	lua_setfield(m_L, -2, "hide");

	lua_pushcfunction(m_L, &game::trap_window_title_fe);
	lua_setfield(m_L, -2, "title");

	lua_pushcfunction(m_L, &game::trap_window_set_title_fe);
	lua_setfield(m_L, -2, "set_title");

	lua_pushcfunction(m_L, &game::trap_window_position_fe);
	lua_setfield(m_L, -2, "position");

	lua_pushcfunction(m_L, &game::trap_window_move_fe);
	lua_setfield(m_L, -2, "move");

	lua_pushcfunction(m_L, &game::trap_window_size_fe);
	lua_setfield(m_L, -2, "size");

	lua_pushcfunction(m_L, &game::trap_window_resize_fe);
	lua_setfield(m_L, -2, "resize");

	lua_setfield(m_L, -2, "window");

	lua_newtable(m_L);

	lua_pushcfunction(m_L, &game::trap_editor_enabled_fe);
	lua_setfield(m_L, -2, "enabled");

	lua_setfield(m_L, -2, "editor");

	lua_setglobal(m_L, "sge");
}

int game::trap_fe(lua_State *L, trap_type tt)
{
	SGE_ASSERT(m_current_trap == TRAP_TYPE_INVALID);
	SGE_ASSERT(m_current_trap_L == NULL);
	SGE_ASSERT(L != NULL);
	SGE_ASSERT(tt != TRAP_TYPE_INVALID);

	m_current_trap = tt;
	m_current_trap_L = L;
	m_current_trap_result = TRAP_RESULT_NOT_SET;

	m_cond.wait(m_mutex);

	SGE_ASSERT(m_current_trap_result != TRAP_RESULT_NOT_SET);

	switch (m_current_trap_result) {
	case TRAP_RESULT_NOT_IMPL:
		luaL_error(L, "trap %d not impl.", tt);
		break;
	}

	return m_current_trap_result;
}

int game::trap_be(lua_State *L, trap_type tt)
{
	int ret;

	switch (tt) {
	case TRAP_TYPE_FPS:
		ret = trap_fps_be(L);
		break;
	case TRAP_TYPE_WINDOW_VISIBLED:
		ret = trap_window_visibled_be(L);
		break;
	case TRAP_TYPE_WINDOW_SHOW:
		ret = trap_window_show_be(L);
		break;
	case TRAP_TYPE_WINDOW_HIDE:
		ret = trap_window_hide_be(L);
		break;
	case TRAP_TYPE_WINDOW_TITLE:
		ret = trap_window_title_be(L);
		break;
	case TRAP_TYPE_WINDOW_SET_TITLE:
		ret = trap_window_set_title_be(L);
		break;
	case TRAP_TYPE_WINDOW_POSITION:
		ret = trap_window_position_be(L);
		break;
	case TRAP_TYPE_WINDOW_MOVE:
		ret = trap_window_move_be(L);
		break;
	case TRAP_TYPE_WINDOW_SIZE:
		ret = trap_window_size_be(L);
		break;
	case TRAP_TYPE_WINDOW_RESIZE:
		ret = trap_window_resize_be(L);
		break;
	case TRAP_TYPE_EDITOR_ENABLED:
		ret = trap_editor_enabled_be(L);
		break;
	default:
		ret = TRAP_RESULT_NOT_IMPL;
		break;
	}

	return ret;
}

int game::trap_version_fe(lua_State *L)
{
	lua_newtable(L);

	lua_pushinteger(L, SGE_VERSION_MAJOR);
	lua_rawseti(L, -2, 0);

	lua_pushinteger(L, SGE_VERSION_MAJOR);
	lua_rawseti(L, -2, 1);

	lua_pushinteger(L, SGE_VERSION_MAJOR);
	lua_rawseti(L, -2, 2);

	return 1;
}

int game::trap_task_fe(lua_State *L)
{
	luaL_argcheck(L, lua_isfunction(L, 1), 1, "not a function");

	lua_State *L1 = lua_newthread(L);
	if (L1 != NULL) {
		lua_pushvalue(L, 1);
		lua_xmove(L, L1, 1);
	} else
		lua_pushnil(L);

	return 1;
}

int game::trap_current_fe(lua_State *L)
{
	if (L == from_lua(L)->m_L)
		luaL_error(L, "Not in a thread");

	lua_pushthread(L);

	return 1;
}

void game::trap_sleep_done(uv_timer_t *timer)
{
	task_t *T = (task_t *)(timer->data);
	SGE_ASSERT(T != NULL);

	game *G = from_task(T);
	SGE_ASSERT(G != NULL);

	sge_list_del(&G->m_task_list_sleep, &T->node);
	sge_list_add_tail(&G->m_task_list, &T->node);
}

int game::trap_sleep_fe(lua_State *L)
{
	int ms = luaL_checkinteger(L, 1);
	if (ms < 0)
		luaL_error(L, "bad arg");

	if (ms > 0) {
		task_t *T = task_from_lua(L);
		SGE_ASSERT(T != NULL);
		game *G = from_task(T);
		SGE_ASSERT(G != NULL);
		uv_timer_start(&T->sleep_timer, &game::trap_sleep_done, ms, 0);
		sge_list_add_tail(&G->m_task_list_sleep, &T->node);
	}

	return lua_yield(L, 0);
}

int game::trap_fps_fe(lua_State *L)
{
	game *G = from_lua(L);
	SGE_ASSERT(G != NULL);

	return G->trap_fe(L, TRAP_TYPE_FPS);
}

int game::trap_fps_be(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_fps)
		return TRAP_RESULT_NOT_IMPL;

	lua_pushinteger(L, trap_fps());

	return 1;
}

int game::trap_window_visibled_fe(lua_State *L)
{
	game *G = from_lua(L);
	SGE_ASSERT(G != NULL);

	return G->trap_fe(L, TRAP_TYPE_WINDOW_VISIBLED);
}

int game::trap_window_visibled_be(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_window_visibled)
		return TRAP_RESULT_NOT_IMPL;

	lua_pushboolean(L, trap_window_visibled());

	return 1;
}

int game::trap_window_show_fe(lua_State *L)
{
	game *G = from_lua(L);
	SGE_ASSERT(G != NULL);

	return G->trap_fe(L, TRAP_TYPE_WINDOW_SHOW);
}

int game::trap_window_show_be(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_window_show)
		return TRAP_RESULT_NOT_IMPL;

	trap_window_show();

	return 0;
}

int game::trap_window_hide_fe(lua_State *L)
{
	game *G = from_lua(L);
	SGE_ASSERT(G != NULL);

	return G->trap_fe(L, TRAP_TYPE_WINDOW_HIDE);
}

int game::trap_window_hide_be(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_window_hide)
		return TRAP_RESULT_NOT_IMPL;

	trap_window_hide();

	return 0;
}

int game::trap_window_title_fe(lua_State *L)
{
	game *G = from_lua(L);
	SGE_ASSERT(G != NULL);

	return G->trap_fe(L, TRAP_TYPE_WINDOW_TITLE);
}

int game::trap_window_title_be(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_window_title)
		return TRAP_RESULT_NOT_IMPL;

	lua_pushstring(L, trap_window_title());

	return 1;
}

int game::trap_window_set_title_fe(lua_State *L)
{
	game *G = from_lua(L);
	SGE_ASSERT(G != NULL);

	return G->trap_fe(L, TRAP_TYPE_WINDOW_SET_TITLE);
}

int game::trap_window_set_title_be(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_window_set_title)
		return TRAP_RESULT_NOT_IMPL;

	trap_window_set_title(luaL_checkstring(L, 1));

	return 0;
}

int game::trap_window_position_fe(lua_State *L)
{
	game *G = from_lua(L);
	SGE_ASSERT(G != NULL);

	return G->trap_fe(L, TRAP_TYPE_WINDOW_POSITION);
}

int game::trap_window_position_be(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_window_position)
		return TRAP_RESULT_NOT_IMPL;

	int x, y;

	trap_window_position(x, y);

	lua_newtable(L);
	lua_pushinteger(L, x);
	lua_rawseti(L, -2, 0);
	lua_pushinteger(L, y);
	lua_rawseti(L, -2, 1);

	return 1;
}

int game::trap_window_move_fe(lua_State *L)
{
	game *G = from_lua(L);
	SGE_ASSERT(G != NULL);

	return G->trap_fe(L, TRAP_TYPE_WINDOW_MOVE);
}

int game::trap_window_move_be(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_window_move)
		return TRAP_RESULT_NOT_IMPL;

	int x = luaL_checkinteger(L, 1);
	int y = luaL_checkinteger(L, 2);

	trap_window_move(x, y);

	return 0;
}

int game::trap_window_size_fe(lua_State *L)
{
	game *G = from_lua(L);
	SGE_ASSERT(G != NULL);

	return G->trap_fe(L, TRAP_TYPE_WINDOW_SIZE);
}

int game::trap_window_size_be(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_window_size)
		return TRAP_RESULT_NOT_IMPL;

	int width, height;

	trap_window_size(width, height);

	lua_newtable(L);
	lua_pushinteger(L, width);
	lua_rawseti(L, -2, 0);
	lua_pushinteger(L, height);
	lua_rawseti(L, -2, 1);

	return 1;
}

int game::trap_window_resize_fe(lua_State *L)
{
	game *G = from_lua(L);
	SGE_ASSERT(G != NULL);

	return G->trap_fe(L, TRAP_TYPE_WINDOW_RESIZE);
}

int game::trap_window_resize_be(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_window_resize)
		return TRAP_RESULT_NOT_IMPL;

	int width = luaL_checkinteger(L, 1);
	int height = luaL_checkinteger(L, 2);

	trap_window_resize(width, height);

	return 0;
}

int game::trap_editor_enabled_fe(lua_State *L)
{
	game *G = from_lua(L);
	SGE_ASSERT(G != NULL);

	return G->trap_fe(L, TRAP_TYPE_EDITOR_ENABLED);
}

int game::trap_editor_enabled_be(lua_State *L)
{
	SGE_ASSERT(L != NULL);

	if (!trap_editor_enabled)
		return TRAP_RESULT_NOT_IMPL;

	lua_pushboolean(L, trap_editor_enabled());

	return 1;
}

SGE_END

