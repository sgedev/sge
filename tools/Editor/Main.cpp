//
//
#include <QApplication>

#include <SGE.hpp>

#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	MainWindow mw;

	mw.init();
	mw.show();

	return app.exec();
}
