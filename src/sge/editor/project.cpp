//
//
#include <QDebug>
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QByteArray>
#include <QTextStream>

#include <sge/editor/project.hpp>

SGE_EDITOR_BEGIN

Project::Project(QObject *parent)
	: QAbstractItemModel(parent)
	, m_state(StateIdle)
{
	connect(&m_gameLauncher, &QProcess::started, this, &Project::launcherStarted);
	connect(&m_gameLauncher, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(launcherFinished(int, QProcess::ExitStatus)));
	connect(&m_gameLauncher, &QProcess::stateChanged, this, &Project::launcherStateChanged);
	//connect(&m_gameLauncher, SIGNAL(QProcess::error(QProcess::ProcessError)), this, SLOT(launcherError(QProcess::ProcessError)));
	connect(&m_gameLauncher, &QProcess::readyReadStandardOutput, this, &Project::launcherStdoutReady);
	connect(&m_gameLauncher, &QProcess::readyReadStandardError, this, &Project::launcherStderrReady);
}

Project::~Project(void)
{
}

QVariant Project::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

Qt::ItemFlags Project::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

	return QAbstractItemModel::flags(index);
}

QVariant Project::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("Comment");
        default:
            break;
        }
    }

    return QVariant();
}

QModelIndex Project::index(int row, int column, const QModelIndex &parent) const
{
    return QModelIndex();
}

QModelIndex Project::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int Project::rowCount(const QModelIndex &parent) const
{
    return 0;
}

int Project::columnCount(const QModelIndex &parent) const
{
	return 2;
}

bool Project::hasChildren(const QModelIndex &parent) const
{
	return QAbstractItemModel::hasChildren(parent);
}

bool Project::insertRows(int row, int count, const QModelIndex &parent)
{
	return QAbstractItemModel::insertRows(row, count, parent);
}

bool Project::removeRows(int row, int count, const QModelIndex &parent)
{
	return QAbstractItemModel::removeRows(row, count, parent);
}

bool Project::create(const QString &path)
{
	return true;
}

bool Project::load(const QString &path)
{
	return true;
}

bool Project::save(void)
{
	return true;
}

void Project::update(float elapsed)
{
	m_game.update(elapsed);
}

void Project::draw(Renderer::View *view)
{
	m_game.draw(view);
}

bool Project::importFile(const QString &import_path, const QString &filename)
{
	return true;
}

bool Project::exportFile(const QString &export_path, const QString &filename)
{
	return true;
}

bool Project::build(void)
{
	return true;
}

void Project::clean(void)
{
}

bool Project::start(const QString &launcher)
{
	QStringList args;

	if (state() != StateReady)
		return false;

	setState(StateStarting);

	return true;
}

bool Project::doLoad(void)
{
	return true;
}

void Project::setState(State st)
{
	if (st == m_state)
		return;

	m_state = st;
	stateChanged(m_state);
}

void Project::launcherStarted(void)
{
	setState(StateRunning);
}

void Project::launcherStateChanged(QProcess::ProcessState newState)
{
	switch (newState) {
	case QProcess::NotRunning:
		setState(StateReady);
		break;
	case QProcess::Starting:
		setState(StateStarting);
		break;
	case QProcess::Running:
		setState(StateRunning);
		break;
	}
}

void Project::launcherError(QProcess::ProcessError error)
{
	switch (error) {
	case QProcess::FailedToStart:
		break;
	case QProcess::Crashed:
		break;
	}
}

void Project::launcherFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	setState(StateReady);
}

void Project::launcherStdoutReady(void)
{
	QString text(m_gameLauncher.readAllStandardOutput());
	launcherOutput(text);
}

void Project::launcherStderrReady(void)
{
	QString text(m_gameLauncher.readAllStandardError());
	launcherError(text);
}

SGE_EDITOR_END

