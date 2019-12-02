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
#include <QHash>
#include <QProcess>
#include <QAbstractItemModel>

#include <sge/filesystemnative.hpp>
#include <sge/game.hpp>
#include <sge/editor/common.hpp>

SGE_EDITOR_BEGIN

class Project: public QAbstractItemModel {
	Q_OBJECT

public:
	enum State {
		StateIdle = 0,
		StateReady,
		StateDirty,
		StateError,
		StateBuilding,
		StateCleanning,
		StateSaving,
		StateLoading,
		StateStarting,
		StateRunning,
	};

public:
	Project(QObject *parent = Q_NULLPTR);
	virtual ~Project(void);

signals:
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

public:
	bool create(const QString &path);
	bool load(const QString &path);
	bool save(void);
	State state(void) const;
	void update(float elapsed);
	void draw(Renderer::View *view);
	bool importFile(const QString &import_path, const QString &filename);
	bool exportFile(const QString &export_path, const QString &filename);
	bool build(void);
	void clean(void);
	bool start(const QString &launcher);
	const QString &path(void) const;

protected:
	bool doLoad(void);
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
	FileSystemNative m_fs;
	Assimp::Importer m_importer;
	Game m_game;
	QProcess m_gameLauncher;
};

inline Project::State Project::state(void) const
{
	return m_state;
}

inline const QString &Project::path(void) const
{
	return m_fs.path();
}

SGE_EDITOR_END

#endif // SGE_EDITOR_PROJECT_HPP
