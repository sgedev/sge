//
//
#include "MainWindow.hpp"

MainWindow::MainWindow(void)
{

}

MainWindow::~MainWindow(void)
{
}

bool MainWindow::init(void)
{
	setWindowTitle("SGE Editor");
	resize(800, 600);
	setCentralWidget(&m_midArea);

	return true;
}
