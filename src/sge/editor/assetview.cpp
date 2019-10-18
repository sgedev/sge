//
//
#include <QString>

#include <sge/editor/assetview.hpp>

SGE_EDITOR_BEGIN

AssetView::AssetView(QWidget *parent)
	: QWidget(parent)
	, m_project(Q_NULLPTR)
{
	m_vbox.addWidget(&m_toolBar);

	m_vbox.addWidget(&m_splitter);
	m_splitter.setOrientation(Qt::Horizontal);

	m_splitter.addWidget(&m_treeView);
	m_treeView.setModel(&m_treeModel);
	m_treeView.setDragDropMode(QAbstractItemView::DragDrop);

	m_splitter.addWidget(&m_listView);

	setLayout(&m_vbox);
}

AssetView::~AssetView(void)
{
}

void AssetView::setProject(Project *project)
{
	m_project = project;
	if (m_project == Q_NULLPTR)
		return;

	QModelIndex index;
	QString assetPath = m_project->assetDir().path();

	//m_treeModel.setFilter(QDir::NoDotAndDotDot);
	index = m_treeModel.setRootPath(assetPath);
	m_treeView.setRootIndex(index);
}

SGE_EDITOR_END

