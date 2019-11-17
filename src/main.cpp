//
//
#include <QCoreApplication>
#include <QStringList>
#include <QSurfaceFormat>

#include <sge.hpp>

static void setupApp(void)
{
	QApplication::setOrganizationName("SGE");

	QApplication::setApplicationVersion(
		QString("%1.%2.%3").arg(SGE_VERSION_MAJOR).arg(SGE_VERSION_MINOR).arg(SGE_VERSION_PATCH));
}

static void setupGuiApp(void)
{
	setupApp();

	// TODO move to renderer.
	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setVersion(4, 0);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
	QSurfaceFormat::setDefaultFormat(format);
}

static QCoreApplication *createLauncherApplication(const QStringList &args)
{
	auto app = new SGE::Launcher::Application();
	if (app == Q_NULLPTR)
		return Q_NULLPTR;

	setupGuiApp();

	if (!app->init(args)) {
		delete app;
		return Q_NULLPTR;
	}

	return app;
}

#ifdef SGE_EDITOR
static QCoreApplication *createEditorApplication(const QStringList &args)
{
	auto app = new SGE::Editor::Application();
	if (app == Q_NULLPTR)
		return Q_NULLPTR;

	setupGuiApp();

	if (!app->init(args)) {
		delete app;
		return Q_NULLPTR;
	}

	return app;
}
#endif

#ifdef SGE_SERVER
static QCoreApplication *createServerApplication(const QStringList &args)
{
	return Q_NULLPTR;
}
#endif

#ifdef SGE_COMPILER
static QCoreApplication *createCompilerApplication(const QStringList &args)
{
	return Q_NULLPTR;
}
#endif

static void printHelp(void)
{
	qInfo("Usage:");
	qInfo("  sge [subcommand]\n");

	qInfo("Subcommands:");

	qInfo("  launch");
	qInfo("    Launch a game, use 'sge launch --help' for details.\n");

#ifdef SGE_EDITOR
	qInfo("  edit");
	qInfo("    Edit a game, use 'sge edit --help' for details.\n");
#endif

#ifdef SGE_SERVER
	qInfo("  serve");
	qInfo("    Start a server for game, use 'sge serve --help' for details.\n");
#endif

#ifdef SGE_COMPILER
	qInfo("  compile");
	qInfo("    Compile a game resource, use 'sge compile --help' for details.\n");
#endif

	qInfo("  help");
	qInfo("    Show this message.\n");
	qInfo("  version");
	qInfo("    Show version message.");
}

static void printVersion(void)
{
	qInfo("%d.%d.%d",
		SGE_VERSION_MAJOR,
		SGE_VERSION_MINOR,
		SGE_VERSION_PATCH);
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printHelp();
		return EXIT_FAILURE;
	}

	QStringList args;
	for (int i = 2; i < argc; ++i)
		args << argv[i];

	QString subcmd = argv[1];
	QCoreApplication *app = Q_NULLPTR;

	if (subcmd == "launch")
		app = createLauncherApplication(QStringList() << "sge launch" << args);
#ifdef SGE_EDITOR
	else if (subcmd == "edit")
		app = createEditorApplication(QStringList() << "sge edit" << args);
#endif
#ifdef SGE_SERVER
	else if (subcmd == "serve")
		app = createServerApplication(QStringList() << "sge serve" << args);
#endif
#ifdef SGE_COMPILER
	else if (subcmd == "compile")
		app = createCompilerApplication(QStringList() << "sge compile" << args);
#endif
	else if (subcmd == "help")
		printHelp();
	else if (subcmd == "version")
		printVersion();
	else
		printHelp();

	if (app == Q_NULLPTR)
		return EXIT_FAILURE;

	int ret = app->exec();
	delete app;

	return ret;
}
