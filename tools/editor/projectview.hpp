//
//
#ifndef SGE_EDITOR_PROJECTVIEW_HPP
#define SGE_EDITOR_PROJECTVIEW_HPP

#include <QTreeView>
#include <QModelIndex>
#include <QHeaderView>

#include "project.hpp"

class ProjectView : public QTreeView {
	Q_OBJECT

public:
	ProjectView(QWidget *parent = Q_NULLPTR);
	virtual ~ProjectView(void);

signals:
	void openItem(const QString &filename);

public:
	bool init(void);
	QPoint mapToGlobal(const QPoint &pos);
	void setProject(Project *project);
	Project *project(void);

protected slots:
	void dirChanged(const QDir &dir);

private:
	QModelIndex m_root;
	Project *m_project;
};

inline Project *ProjectView::project(void)
{
	return m_project;
}

#endif // SGE_EDITOR_PROJECTVIEW_HPP
