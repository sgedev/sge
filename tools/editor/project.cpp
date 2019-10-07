//
//
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
	if (!d.path().isEmpty()) {
		if (!d.exists())
			return false;

		QDir::setCurrent(d.path());

		QFile version_file(d.path() + "/version");
		if (!version_file.open(QIODevice::ReadWrite)) {
			printf("failed to create version.\n");
		}

		version_file.write("SGEV1\n", 6);
		version_file.close();

		QString vroot = "/" + d.dirName();

		if (!m_game.init(vroot.toStdString().c_str()))
			return false;
	}

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

