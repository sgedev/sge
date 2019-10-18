//
//
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QStringList>
#include <QSurfaceFormat>
#include <QMainWindow>
#include <QFont>

#include <sge.hpp>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QApplication::setOrganizationName("SGE");
	QApplication::setApplicationVersion("0.1");

	app.setFont(QFont("Mono"));

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setVersion(4, 0);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
	QSurfaceFormat::setDefaultFormat(format);

	QCommandLineParser parser;

    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("<game path>", "Specify the game path, default is current working directory.");

	QString modeDesc = "Specify the running mode: 'launcher'(defualt)";

#ifdef SGE_EDITOR
	modeDesc += ", 'editor'";
#endif

#ifdef SGE_SERVER
	modeDesc += ", 'server'";
#endif

	modeDesc += ".";

    QCommandLineOption modeOption(QStringList() << "m" << "mode", modeDesc, "mode");
    parser.addOption(modeOption);

    parser.process(app);

    const QStringList args = parser.positionalArguments();
	QString mode = parser.value(modeOption);

	QMainWindow *mainWindow = Q_NULLPTR;

	if (mode.isEmpty() || mode == "launcher") {
		QApplication::setApplicationName("Launcher");
		qInfo() << "Starting launcher...\n";
		mainWindow = new SGE::Launcher::MainWindow();
#ifdef SGE_EDITOR
	} else if (mode == "editor") {
		QApplication::setApplicationName("Editor");
		qInfo() << "Starting editor...\n";
		mainWindow = new SGE::Editor::MainWindow();
#endif
#ifdef SGE_SERVER
	} else if (mode == "server") {
		QApplication::setApplicationName("Server");
		qInfo() << "Starting server...\n";
		// TODO
#endif
	} else {
		qCritical() << "Unknown mode:" << mode;
		parser.showHelp();
	}

	if (mainWindow == Q_NULLPTR)
		return EXIT_FAILURE;

	mainWindow->show();

	return app.exec();
}
