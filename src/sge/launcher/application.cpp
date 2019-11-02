//
//
#include <QCommandLineParser>
#include <QCommandLineOption>

#include <sge/launcher/application.hpp>

SGE_LAUNCHER_BEGIN

static int argc = 1;
static char *argv = "sge launch";

Application::Application(void)
	: QGuiApplication(argc, &argv)
{
	setApplicationName("Launcher");
}

Application::~Application(void)
{
}

bool Application::init(const QStringList &args)
{
	QCommandLineParser parser;

	parser.addPositionalArgument("game path", "Specify the game path");
	parser.addHelpOption();
	parser.process(args);

	m_mainWindow.show();

	return true;
}

SGE_LAUNCHER_END
