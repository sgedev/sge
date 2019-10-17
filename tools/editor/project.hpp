//
//
#ifndef SGE_EDITOR_PROJECT_HPP
#define SGE_EDITOR_PROJECT_HPP

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <QVariant>
#include <QString>
#include <QEvent>
#include <QList>
#include <QDir>
#include <QHash>
#include <QDomNode>
#include <QDomElement>
#include <QDomDocument>
#include <QProcess>
#include <QAbstractItemModel>

#include "game.hpp"

class Project: public QAbstractItemModel {
	Q_OBJECT

public:
	class Item {
	public:
		enum Type {
			TYPE_UNKNOWN = 0,
			TYPE_ASSETS,
			TYPE_ASSET,
			TYPE_SCENES,
			TYPE_SCENE,
			TYPE_LEVELS,
			TYPE_LEVEL,
		};

	public:
		Item(const QDomNode &node, int row, Item *parent = nullptr, Type type = TYPE_UNKNOWN);
		~Item();

	public:
		Item *child(int i);
		Item *parent(void);
		QDomNode node(void) const;
		int row(void) const;
		Type type(void) const;

	private:
		QDomNode m_domNode;
		QHash<int, Item *> m_childItems;
		Item *m_parentItem;
		int m_rowNumber;
		Type m_type;
	};

public:
	enum State {
		STATE_IDLE = 0,
		STATE_READY,
		STATE_DIRTY,
		STATE_ERROR,
		STATE_BUILDING,
		STATE_CLEANNING,
		STATE_SAVING,
		STATE_LOADING,
		STATE_STARTING,
		STATE_RUNNING,
	};

public:
	Project(QObject *parent = Q_NULLPTR);
	virtual ~Project(void);

signals:
	void dirChanged(const QDir &d);
	void stateChanged(State state);
	void buildOutput(const QString &text);
	void launcherError(const QString &text);
	void launcherOutput(const QString &text);

public:
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
	bool hasChildren(const QModelIndex &parent) const override;
	bool insertRows(int row, int count, const QModelIndex &parent) override;
	bool removeRows(int row, int count, const QModelIndex &parent) override;
	Item &item(const QModelIndex &index);

public:
	void reset(void);
	State state(void) const;
	bool setup(const QDir &dir);
	bool handleEvent(const QEvent *evt);
	void update(float elapsed);
	void draw(SGE::View *v);
	bool save(const QDir &d);
	bool load(const QDir &d);
	bool importFile(const QString &import_path, const QString &filename);
	bool exportFile(const QString &export_path, const QString &filename);
	bool build(void);
	void clean(void);
	bool start(const QString &launcher);
	const QDir &dir(void) const;
	const QDir &outDir(void) const;

protected:
	void setDir(const QDir &d);
	void setState(State st);

protected slots:
	void launcherStarted(void);
	void launcherFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void launcherStateChanged(QProcess::ProcessState newState);
	void launcherError(QProcess::ProcessError error);
	void launcherStdoutReady(void);
	void launcherStderrReady(void);

private:
	State m_state;
	QDir m_dir;
	QDir m_outDir;
	QDir m_assetDir;
	QDomDocument m_manifest;
	Item *m_rootItem;
	ttvfs::Root m_rootFS;
	Assimp::Importer m_importer;
	Game m_game;
	QProcess m_gameLauncher;
};

inline Project::Item::Item(const QDomNode &node, int row, Item *parent, Type type)
    : m_domNode(node)
	, m_parentItem(parent)
	, m_rowNumber(row)
	, m_type(type)
{
}

inline Project::Item::~Item(void)
{
    qDeleteAll(m_childItems);
}

inline Project::Item *Project::Item::parent(void)
{
    return m_parentItem;
}

inline int Project::Item::row(void) const
{
    return m_rowNumber;
}

inline QDomNode Project::Item::node(void) const
{
    return m_domNode;
}

inline Project::Item::Type Project::Item::type(void) const
{
	return m_type;
}

inline Project::State Project::state(void) const
{
	return m_state;
}

inline const QDir &Project::dir(void) const
{
	return m_dir;
}

inline const QDir &Project::outDir(void) const
{
	return m_outDir;
}

#endif // SGE_EDITOR_PROJECT_HPP
