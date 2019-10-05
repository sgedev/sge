//
//
#include <QApplication>
#include <QFont>

#include <physfs.h>
#include <sge.hpp>

#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QCoreApplication::setOrganizationName("SGE");
	QCoreApplication::setApplicationName("Editor");

	app.setFont(QFont("Courier New"));

	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
	QSurfaceFormat::setDefaultFormat(format);

	if (!PHYSFS_init(NULL))
		return EXIT_FAILURE;

	MainWindow mw;

	mw.Init();
	mw.setWindowTitle("SGE Editor");
	mw.show();

	int ret = app.exec();

	PHYSFS_deinit();

	return ret;
}
