//
//
#ifndef SGE_LAUNCHER_APPLICATION_HPP
#define SGE_LAUNCHER_APPLICATION_HPP

#include <QApplication>
#include <QStringList>

#include <sge/launcher/common.hpp>
#include <sge/launcher/mainwindow.hpp>

SGE_LAUNCHER_BEGIN

class Application : public QGuiApplication {
	Q_OBJECT

public:
	Application(void);
	virtual ~Application(void);

public:
	bool init(const QStringList &args);

private:
	MainWindow m_mainWindow;
};

SGE_LAUNCHER_END

#endif // SGE_LAUNCHER_APPLICATION_HPP
