//
//
#ifndef SGE_EDITOR_PROJECTVIEW_HPP
#define SGE_EDITOR_PROJECTVIEW_HPP

#include <QTreeWidget>

#include "project.hpp"

class ProjectView : public QTreeWidget {
public:
	ProjectView(QWidget *parent = Q_NULLPTR);
	virtual ~ProjectView(void);

public:
	void setProject(Project *project);
	Project *project(void);

private:
	Project *m_project;
};

inline Project *ProjectView::project(void)
{
	return m_project;
}

#endif // SGE_EDITOR_PROJECTVIEW_HPP
