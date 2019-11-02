//
//
#ifndef SGE_EDITOR_APPLICATION_HPP
#define SGE_EDITOR_APPLICATION_HPP

#include <QApplication>
#include <QStringList>

#include <sge/editor/common.hpp>
#include <sge/editor/mainwindow.hpp>

SGE_EDITOR_BEGIN

class Application: public QApplication {
	Q_OBJECT

public:
	Application(void);
	virtual ~Application(void);

public:
	bool init(const QStringList &args);

private:
	MainWindow m_mainWindow;
};

SGE_EDITOR_END

#endif // SGE_EDITOR_APPLICATION_HPP
