//
//
#include <QCommandLineParser>
#include <QCommandLineOption>

#include <sge/editor/application.hpp>

SGE_EDITOR_BEGIN

static int argc = 1;
static char *argv = "sge edit";

Application::Application(void)
	: QApplication(argc, (char **)&argv)
{
	setApplicationName("Editor");
	//setFont(QFont("Mono"));
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

SGE_EDITOR_END
