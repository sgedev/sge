//
//
#ifndef SGE_EDITOR_NEWSCRIPTDIALOG_HPP
#define SGE_EDITOR_NEWSCRIPTDIALOG_HPP

#include <QDialog>

#include <sge/editor/common.hpp>

SGE_EDITOR_BEGIN

class NewScriptDialog: public QDialog {
	Q_OBJECT

public:
	NewScriptDialog(QWidget *parent = Q_NULLPTR);
	virtual ~NewScriptDialog(void);
};

SGE_EDITOR_END

#endif // SGE_EDITOR_NEWSCRIPTDIALOG_HPP

