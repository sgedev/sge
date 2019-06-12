//
//
#ifndef SGE_EDITOR_MAIN_WINDOW_H
#define SGE_EDITOR_MAIN_WINDOW_H

#include <QMainWindow>
#include <QMdiArea>

#include <SGE/Editor/Common.h>

SGE_EDITOR_BEGIN

class MainWindow: public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	virtual ~MainWindow(void);

public:
	bool init(void);

protected:
	virtual void closeEvent(QCloseEvent *event);

private:
	QMdiArea m_mdiArea;
};

SGE_EDITOR_END

#endif // SGE_EDITOR_MAIN_WINDOW_H

