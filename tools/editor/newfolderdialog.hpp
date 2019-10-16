//
//
#ifndef SGE_EDITOR_NEWFOLDERDIALOG_HPP
#define SGE_EDITOR_NEWFOLDERDIALOG_HPP

#include <QDialog>

class NewFolderDialog: public QDialog {
	Q_OBJECT

public:
	NewFolderDialog(QWidget *parent = Q_NULLPTR);
	virtual ~NewFolderDialog(void);
};

#endif // SGE_EDITOR_NEWFOLDERDIALOG_HPP

