//
//
#include <QDebug>
#include <QStringList>
#include <QFile>
#include <QByteArray>
#include <QTextStream>

#include "project.hpp"

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
	, m_state(STATE_IDLE)
	, m_rootItem(Q_NULLPTR)
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
	if (m_rootItem != Q_NULLPTR)
		delete m_rootItem;
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
    if (!hasIndex(row, column, parent))
        return QModelIndex();

	Item *parentItem;

	if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<Item *>(parent.internalPointer());

    Item *childItem = parentItem->child(row);
    if (childItem != Q_NULLPTR)
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex Project::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    Item *childItem = static_cast<Item *>(child.internalPointer());
    Item *parentItem = childItem->parent();

    if (parentItem == NULL || parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int Project::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    Item *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<Item *>(parent.internalPointer());

    return parentItem->node().childNodes().count();
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
	m_manifest.clear();
}

bool Project::setup(const QDir &dir)
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

	m_outDir.setPath(dir.path() + "/out");
	dir.mkpath(m_outDir.path());

	m_assetDir.setPath(dir.path() + "/asset");
	dir.mkpath(m_assetDir.path());

	QDomDocument manifest;
	QDomElement rootNode = manifest.createElement("SGE");
	rootNode.setAttribute("Version", "0.1");
	manifest.appendChild(rootNode);

	rootNode.appendChild(manifest.createElement("Assets"));
	rootNode.appendChild(manifest.createElement("Scenes"));
	rootNode.appendChild(manifest.createElement("Levels"));

	QFile file(dir.path() + "/manifest.xml");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;

	QTextStream s(&file);
	s << manifest.toString();
	qDebug() << manifest.toString();
	file.close();

	m_manifest = manifest;
	m_rootItem = new Item(rootNode, 0);

	m_rootFS.AddLoader(new ttvfs::DiskLoader);
	m_rootFS.Mount(m_outDir.path().toStdString().c_str(), "");

	m_game.init(&m_rootFS);

	setDir(dir);
	setState(STATE_READY);

	return true;
}

bool Project::handleEvent(const QEvent *evt)
{
	if (state() == STATE_RUNNING)
		return m_game.handleEvent(evt);
}

void Project::update(float elapsed)
{
	m_game.update(elapsed);
}

void Project::draw(SGE::View *v)
{
	m_game.draw(v);
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

	if (state() != STATE_READY)
		return false;

	setState(STATE_STARTING);

	args << m_outDir.path();
	m_gameLauncher.start(launcher, args);
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
	setState(STATE_RUNNING);
}

void Project::launcherStateChanged(QProcess::ProcessState newState)
{
	switch (newState) {
	case QProcess::NotRunning:
		setState(STATE_READY);
		break;
	case QProcess::Starting:
		setState(STATE_STARTING);
		break;
	case QProcess::Running:
		setState(STATE_RUNNING);
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
	setState(STATE_READY);
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

