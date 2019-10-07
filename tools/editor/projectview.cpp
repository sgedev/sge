//
//
#include <QContextMenuEvent>

#include "projectview.hpp"

ProjectView::ProjectView(QWidget *parent)
	: QTreeView(parent)
{
	setWindowTitle("Project");

	setModel(&m_fs);
}

ProjectView::~ProjectView(void)
{

}

bool ProjectView::init(void)
{
	return true;
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

