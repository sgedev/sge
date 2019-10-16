//
//
#include <QDebug>
#include <QFile>

#include "project.hpp"

Project::Project(QObject *parent)
	: QObject(parent)
{
}

Project::~Project(void)
{
}

void Project::reset(void)
{
	m_game.shutdown();
}

bool Project::setup(const QDir &d)
{
	if (d.path().isEmpty()) {
		m_dir = d;
		dirChanged(d);
		return true;
	}

	if (!d.exists())
		return false;

	QDir::setCurrent(d.path());

	m_root.AddLoader(new ttvfs::DiskLoader);
	m_root.Mount(d.path().toStdString().c_str(), "");

	m_game.init(&m_root);

	m_dir = d;
	dirChanged(d);

	return true;
}

bool Project::handleEvent(const QEvent *evt)
{
	return m_game.handleEvent(evt);
}

void Project::update(float elapsed)
{
	m_game.update(elapsed);
}

void Project::draw(SGE::View *v)
{
	m_game.draw(v);
}

bool Project::save(const QDir &d)
{
	return true;
}

bool Project::load(const QDir &d)
{
	return true;
}

bool Project::importFile(const QString &import_path, const QString &filename)
{
	return true;
}

bool Project::exportFile(const QString &export_path, const QString &filename)
{
	return true;
}

