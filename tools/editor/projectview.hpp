//
//
#ifndef SGE_EDITOR_PROJECTVIEW_HPP
#define SGE_EDITOR_PROJECTVIEW_HPP

#include <QTreeView>
#include <QModelIndex>
#include <QFileSystemModel>

#include "project.hpp"

class ProjectView : public QTreeView {
	Q_OBJECT

public:
	ProjectView(QWidget *parent = Q_NULLPTR);
	virtual ~ProjectView(void);

public:
	bool init(void);
	void setProject(Project *project);
	Project *project(void);

protected slots:
	void dirChanged(const QDir &dir);

private:
	QFileSystemModel m_fs;
	QModelIndex m_root;
	Project *m_project;
};

inline Project *ProjectView::project(void)
{
	return m_project;
}

#endif // SGE_EDITOR_PROJECTVIEW_HPP
