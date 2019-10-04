//
//
#ifndef SGE_EDITOR_CONSOLEVIEW_HPP
#define SGE_EDITOR_CONSOLEVIEW_HPP

#include <QTextEdit>

class ConsoleView: public QTextEdit {
public:
	ConsoleView(QWidget *parent = Q_NULLPTR);
	virtual ~ConsoleView(void);
};

#endif // SGE_EDITOR_CONSOLEVIEW_HPP
