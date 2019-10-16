//
//
#include <QDebug>
#include <QFileInfo>
#include <QContextMenuEvent>

#include "projectview.hpp"

ProjectView::ProjectView(QWidget *parent)
	: QTreeView(parent)
{
	setWindowTitle("Project");
	setSelectionMode(QAbstractItemView::SingleSelection);
	setModel(&m_fs);

	connect(this, &QAbstractItemView::doubleClicked, this, &ProjectView::openItem);
	connect(this, &QWidget::customContextMenuRequested, this, &ProjectView::contextMenuRequested);
}

ProjectView::~ProjectView(void)
{
}

bool ProjectView::init(void)
{
	return true;
}

QPoint ProjectView::mapToGlobal(const QPoint &pos)
{
	QPoint gpos;

	if (!isHeaderHidden()) {
		gpos.setX(pos.x());
		gpos.setY(pos.y() + header()->height());
	} else {
		gpos.setX(pos.x());
		gpos.setY(pos.y());
	}

	return QTreeView::mapToGlobal(gpos);
}

void ProjectView::setProject(Project *project)
{
	m_project = project;
	if (m_project == NULL)
		return;

	connect(project, &Project::dirChanged, this, &ProjectView::dirChanged);
	dirChanged(m_project->dir());
}

void ProjectView::dirChanged(const QDir &dir)
{
	m_root = m_fs.setRootPath(dir.path());
	setRootIndex(m_root);
}

void ProjectView::openItem(const QModelIndex &index)
{
	if (!index.isValid())
		return;

	if (m_fs.isDir(index))
		return;

	openFile(m_fs.fileInfo(index).absolutePath());
}

void ProjectView::contextMenuRequested(const QPoint &pos)
{
	QModelIndex index = indexAt(pos);
	if (!index.isValid())
		customOtherContextMenuRequested(pos);
	else if (m_fs.isDir(index))
		customFolderContextMenuRequested(pos);
	else
		customFileContextMenuRequested(pos);
}

