//
//
#ifndef SGE_EDITOR_ASSERTVIEW_HPP
#define SGE_EDITOR_ASSERTVIEW_HPP

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeView>
#include <QListView>
#include <QToolBar>
#include <QSplitter>
#include <QFileSystemModel>

#include <sge/editor/common.hpp>
#include <sge/editor/project.hpp>

SGE_EDITOR_BEGIN

class AssetView: public QWidget {
	Q_OBJECT

public:
	AssetView(QWidget *parent = Q_NULLPTR);
	virtual ~AssetView(void);

public:
	void setProject(Project *project);

private:
	Project *m_project;
	QVBoxLayout m_vbox;
	QToolBar m_toolBar;
	QSplitter m_splitter;
	QTreeView m_treeView;
	QFileSystemModel m_treeModel;
	QListView m_listView;
};

SGE_EDITOR_END

#endif // SGE_EDITOR_ASSERTVIEW_HPP

