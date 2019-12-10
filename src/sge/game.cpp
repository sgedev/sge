//
//
#include <sge/game.hpp>

SGE_BEGIN

Game::Game(void)
	: m_state(StateIdle)
{
}

Game::~Game(void)
{
}

void Game::update(float elapsed)
{
	switch (m_state) {
	case StateIdle:
		break;
	}
}

void Game::draw(Renderer::View *view)
{
	Q_ASSERT(view != Q_NULLPTR);

	switch (m_state) {
	case StateIdle:
		break;
	}
}

JSValue Game::jsFps(JSContext *ctx, JSValueConst thisVal, int argc, JSValueConst *argv)
{
	Game *game = (Game *)JS_GetContextOpaque(ctx);
	Q_ASSERT(game != Q_NULLPTR);

	return JS_NewInt32(ctx, game->trapFps());
}

JSValue Game::jsMaxFps(JSContext *ctx, JSValueConst thisVal, int argc, JSValueConst *argv)
{
	Game *game = (Game *)JS_GetContextOpaque(ctx);
	Q_ASSERT(game != Q_NULLPTR);

	return JS_NewInt32(ctx, game->trapMaxFps());
}

JSValue Game::jsSetMaxFps(JSContext *ctx, JSValueConst thisVal, int argc, JSValueConst *argv)
{
	int n;

    if (JS_ToInt32(ctx, &n, argv[0]))
        return JS_EXCEPTION;

	Game *game = (Game *)JS_GetContextOpaque(ctx);
	Q_ASSERT(game != Q_NULLPTR);

	return JS_NewInt32(ctx, game->trapSetMaxFps(n));
}

int Game::initExports(JSContext *ctx, JSModuleDef *m)
{
    JSValue jsFpsFunc = JS_NewCFunction(ctx, &Game::jsFps, "fps", 0);
	JS_SetModuleExport(ctx, m, "fps", jsFpsFunc);

	JSValue jsMaxFpsFunc = JS_NewCFunction(ctx, &Game::jsMaxFps, "maxFps", 0);
	JS_SetModuleExport(ctx, m, "maxFps", jsMaxFpsFunc);

	JSValue jsSetMaxFpsFunc = JS_NewCFunction(ctx, &Game::jsSetMaxFps, "setMaxFps", 1);
	JS_SetModuleExport(ctx, m, "setFpsMax", jsSetMaxFpsFunc);

    return 0;
}

void Game::run(void)
{
	JSRuntime *runtime;
	JSContext *context;
	JSModuleDef *module;

	runtime = JS_NewRuntime();
	if (runtime == NULL)
		goto end0;

	context = JS_NewContext(runtime);
	if (context == NULL)
		goto end1;

	JS_SetContextOpaque(context, this);

	module = JS_NewCModule(context, "sge", &Game::initExports);
    if (module == NULL)
        goto end2;

    JS_AddModuleExport(context, module, "sge");

	QThread::run();

end2:
	JS_FreeContext(context);
	context = NULL;

end1:
	JS_FreeRuntime(runtime);
	runtime = NULL;

end0:
	return;
}

SGE_END

