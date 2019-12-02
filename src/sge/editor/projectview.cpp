//
//
#include <QDebug>
#include <QFileInfo>
#include <QContextMenuEvent>

#include <sge/editor/projectview.hpp>

SGE_EDITOR_BEGIN

ProjectView::ProjectView(QWidget *parent)
	: QTreeView(parent)
{
	setWindowTitle("Project");
	setSelectionMode(QAbstractItemView::SingleSelection);
	setModel(NULL);
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

	setModel(m_project);

	//connect(m_project, &Project::dirChanged, this, &ProjectView::dirChanged);
	//dirChanged(m_project->dir());
}

void ProjectView::dirChanged(const QDir &dir)
{
	
}

SGE_EDITOR_END

