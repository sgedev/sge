//
//
#include <QStringList>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QApplication>

#include <SGE/Game.h>
#include <SGE/Editor.h>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QApplication::setApplicationName("SGE");
    QApplication::setApplicationVersion("0.1");

	QCommandLineParser parser;
    parser.setApplicationDescription("SGE");

    parser.addHelpOption();

    parser.addVersionOption();

    parser.addPositionalArgument("game-path", QApplication::translate("main", "Game path."));

	QCommandLineOption opt_edit(QStringList() << "e" << "edit-mode",
		QCoreApplication::translate("main", "Edit mode."));
    parser.addOption(opt_edit);

	parser.process(app);

	return app.exec();
}

