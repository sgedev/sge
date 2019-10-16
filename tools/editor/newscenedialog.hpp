//
//
#ifndef SGE_EDITOR_NEWSCENEDIALOG_HPP
#define SGE_EDITOR_NEWSCENEDIALOG_HPP

#include <QDialog>

class NewSceneDialog: public QDialog {
	Q_OBJECT

public:
	NewSceneDialog(QWidget *parent = Q_NULLPTR);
	virtual ~NewSceneDialog(void);
};

#endif // SGE_EDITOR_NEWSCENEDIALOG_HPP

