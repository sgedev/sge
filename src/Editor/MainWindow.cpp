//
//
#include <QSettings>

#include <SGE/Editor/MainWindow.h>

SGE_EDITOR_BEGIN

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
}

MainWindow::~MainWindow(void)
{
}

bool MainWindow::init(void)
{
	setWindowTitle("SGE Editor");

	QSettings settings;
	restoreGeometry(settings.value("MainWindow/Geometry").toByteArray());
	restoreGeometry(settings.value("MainWindow/State").toByteArray());

	setCentralWidget(&m_mdiArea);

	return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	QSettings settings;
	settings.setValue("MainWindow/Geometry", saveGeometry());
	settings.setValue("MainWindow/State", saveState());
	QMainWindow::closeEvent(event);
}

SGE_EDITOR_END

