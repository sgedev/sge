//
//
#include "projectview.hpp"

ProjectView::ProjectView(QWidget *parent)
	: QTreeWidget(parent)
{
}

ProjectView::~ProjectView(void)
{

}

void ProjectView::setProject(Project *project)
{
	m_project = project;
}
