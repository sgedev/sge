//
//
#include <QApplication>
#include <sge.hpp>

#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	MainWindow mw;

	mw.resize(800, 600);
	mw.show();

	return app.exec();
}

