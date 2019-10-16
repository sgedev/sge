//
//
#ifndef SGE_EDITOR_PROJECT_HPP
#define SGE_EDITOR_PROJECT_HPP

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <QEvent>
#include <QList>
#include <QDir>
#include <QDomDocument>

#include "game.hpp"

class Project: public QObject {
	Q_OBJECT

public:
	Project(QObject *parent = Q_NULLPTR);
	virtual ~Project(void);

signals:
	void dirChanged(const QDir &d);

public:
	void reset(void);
	bool setup(const QDir &d);
	bool handleEvent(const QEvent *evt);
	void update(float elapsed);
	void draw(SGE::View *v);
	bool save(const QDir &d);
	bool load(const QDir &d);
	bool importFile(const QString &import_path, const QString &filename);
	bool exportFile(const QString &export_path, const QString &filename);
	Game &game(void);
	const QDir &dir(void) const;

private:
	QDomDocument m_manifest;
	Game m_game;
	ttvfs::Root m_root;
	QDir m_dir;
	Assimp::Importer m_importer;
};

inline Game &Project::game(void)
{
	return m_game;
}

inline const QDir &Project::dir(void) const
{
	return m_dir;
}

#endif // SGE_EDITOR_PROJECT_HPP
