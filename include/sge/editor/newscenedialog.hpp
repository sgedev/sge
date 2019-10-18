//
//
#ifndef SGE_EDITOR_NEWSCENEDIALOG_HPP
#define SGE_EDITOR_NEWSCENEDIALOG_HPP

#include <QDialog>

#include <sge/editor/common.hpp>

SGE_EDITOR_BEGIN

class NewSceneDialog: public QDialog {
	Q_OBJECT

public:
	NewSceneDialog(QWidget *parent = Q_NULLPTR);
	virtual ~NewSceneDialog(void);
};

SGE_EDITOR_END

#endif // SGE_EDITOR_NEWSCENEDIALOG_HPP

