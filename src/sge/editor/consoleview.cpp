//
//
#include <sge/editor/consoleview.hpp>

SGE_EDITOR_BEGIN

ConsoleView::ConsoleView(QWidget *parent)
	: QTextEdit(parent)
{
	setWindowTitle("Console");
}

ConsoleView::~ConsoleView(void)
{

}

SGE_EDITOR_END

