//
//
#ifndef SGE_EDITOR_CONSOLEVIEW_HPP
#define SGE_EDITOR_CONSOLEVIEW_HPP

#include <QTextEdit>

#include <sge/editor/common.hpp>

SGE_EDITOR_BEGIN

class ConsoleView: public QTextEdit {
public:
	ConsoleView(QWidget *parent = Q_NULLPTR);
	virtual ~ConsoleView(void);
};

SGE_EDITOR_END

#endif // SGE_EDITOR_CONSOLEVIEW_HPP
