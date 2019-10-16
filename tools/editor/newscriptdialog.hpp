//
//
#ifndef SGE_EDITOR_NEWSCRIPTDIALOG_HPP
#define SGE_EDITOR_NEWSCRIPTDIALOG_HPP

#include <QDialog>

class NewScriptDialog: public QDialog {
	Q_OBJECT

public:
	NewScriptDialog(QWidget *parent = Q_NULLPTR);
	virtual ~NewScriptDialog(void);
};

#endif // SGE_EDITOR_NEWSCRIPTDIALOG_HPP

