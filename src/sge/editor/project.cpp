//
//
#include <QDebug>
#include <QStringList>
#include <QFile>
#include <QByteArray>
#include <QTextStream>

#include <sge/editor/project.hpp>

SGE_EDITOR_BEGIN

Project::Item *Project::Item::child(int i)
{
    Project::Item *childItem = m_childItems.value(i);
    if (childItem != NULL)
        return childItem;

    // if child does not yet exist, create it
    if (i >= 0 && i < m_domNode.childNodes().count()) {
        QDomNode childNode = m_domNode.childNodes().item(i);
        childItem = new Item(childNode, i, this);
        m_childItems[i] = childItem;
    }

    return childItem;
}

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
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    const Item *item = static_cast<Item *>(index.internalPointer());

    const QDomNode node = item->node();

    switch (index.column()) {
    case 0:
        return node.nodeName();
	case 1:
		return node.nodeValue();
    }

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

void Project::reset(void)
{
	m_game.shutdown();
}

bool Project::create(const QDir &dir)
{
	if (dir.path().isEmpty()) {
		m_dir = dir;
		dirChanged(dir);
		return true;
	}

	if (!dir.exists())
		return false;

	reset();

	QDir::setCurrent(dir.path());

	m_dbfs.init(dir.path());
	m_db.load(&m_dbfs);
	//m_db.reset();

	Database::Group root = m_db.root();
	//Database::Group assets = root.createChildGroup("Assets");
	//Database::Asset a1 = assets.createChildAsset("Box");
	//Database::Value v1 = assets.createChildValue("Version");
	//v1.setInt(123);
	//Database::Group scenes = root.createChildGroup("Scenes");
	//Database::Group levels = root.createChildGroup("Levels");

	//m_db.save();

	Database::Group g1 = root.firstChildGroup("Assets");
	Database::Value v2 = g1.firstChildValue("Version");
	qDebug() << v2.toBool();
	qDebug() << v2.toInt();
	qDebug() << v2.toDouble();
	qDebug() << v2.toString();

	m_game.init(&m_db);

	setDir(dir);
	setState(StateReady);

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

bool Project::save(const QDir &d)
{
	return true;
}

bool Project::load(const QDir &d)
{
	return true;
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

void Project::setDir(const QDir &d)
{
	if (d == m_dir)
		return;

	m_dir = d;
	dirChanged(m_dir);
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

