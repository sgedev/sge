//
//
#include "consoleview.hpp"

ConsoleView::ConsoleView(QWidget *parent)
	: QTextEdit(parent)
{
	setWindowTitle("Console");
}

ConsoleView::~ConsoleView(void)
{

}
