//
//
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QSharedPointer>
#include <QMdiArea>
#include <QMainWindow>

#include <SGE/Game.hpp>

typedef QSharedPointer<SGE::Game> GamePtr;

class MainWindow: public QMainWindow {
	Q_OBJECT

public:
	MainWindow(void);
	virtual ~MainWindow(void);

public:
	bool init(void);

private:
	QMdiArea m_midArea;
	GamePtr m_game;
};

#endif // MAINWINDOW_HPP
