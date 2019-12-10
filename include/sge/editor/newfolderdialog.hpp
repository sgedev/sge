//
//
#ifndef SGE_EDITOR_NEWFOLDERDIALOG_HPP
#define SGE_EDITOR_NEWFOLDERDIALOG_HPP

#include <QDialog>

#include <sge/editor/common.hpp>

SGE_EDITOR_BEGIN

class NewFolderDialog: public QDialog {
	Q_OBJECT

public:
	NewFolderDialog(QWidget *parent = Q_NULLPTR);
	virtual ~NewFolderDialog(void);
};

SGE_EDITOR_END

#endif // SGE_EDITOR_NEWFOLDERDIALOG_HPP

