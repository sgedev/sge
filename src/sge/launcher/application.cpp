//
//
#include <QDir>
#include <QFileInfo>
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

	const QStringList pargs = parser.positionalArguments();
	if (pargs.count() != 1)
		return false;

	QFileInfo fi(pargs.at(0));
	if (!fi.isReadable())
		return false;

	QString filename = QDir::fromNativeSeparators(pargs.at(0));
	qDebug() << filename;

	if (!m_mainWindow.init(filename))
		return false;

	m_mainWindow.show();

	return true;
}

SGE_LAUNCHER_END
