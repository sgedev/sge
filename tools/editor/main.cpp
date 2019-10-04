//
//
#include <QApplication>
#include <sge.hpp>

#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
	QSurfaceFormat::setDefaultFormat(format);

	MainWindow mw;

	mw.Init();
	mw.setWindowTitle("SGE Editor");
	mw.resize(800, 600);
	mw.show();

	return app.exec();
}
