//
//
#include <QByteArray>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <sge/game.hpp>

SGE_BEGIN

Game::Game(void)
	: m_versionMajor(0)
	, m_versionMinor(0)
	, m_state(StateIdle)
{
}

Game::~Game(void)
{
}

bool Game::init(FileSystem *fs)
{
	Q_ASSERT(fs != Q_NULLPTR);

	if (m_state != StateIdle)
		return false;

	if (fs->isReadonly())
		return false;

	QJsonObject root;
	
	root["Version"] = QString("%1.%2").arg(SGE_VERSION_MAJOR).arg(SGE_VERSION_MINOR);
	root["Assets"] = QJsonArray();
	root["Scenes"] = QJsonArray();
	root["Levels"] = QJsonArray();

	QJsonDocument manifest;
	manifest.setObject(root);

	FilePtr file = fs->openManifest(QIODevice::WriteOnly | QIODevice::Truncate);
	if (!file)
		return false;

	QByteArray data = manifest.toJson();
	if (data.isNull() || data.isEmpty())
		return false;

	qint64 size = file->write(data);
	if (size != data.size())
		return false;

	QStringList list = fs->archiveList();
	for (auto it(list.begin()); it != list.end(); ++it)
		fs->removeArchive(*it);

	return load(fs);
}

bool Game::load(FileSystem *fs)
{
	Q_ASSERT(fs != Q_NULLPTR);

	if (m_state != StateIdle)
		return false;

	FilePtr file = fs->openManifest(QIODevice::ReadOnly);
	if (!file)
		return false;

	QByteArray data = file->readAll();
	if (data.isNull() || data.isEmpty())
		return false;

	QJsonDocument manifest = QJsonDocument::fromJson(data);
	if (!manifest.isObject())
		return false;

	QJsonObject root = manifest.object();
	if (root.isEmpty())
		return false;

	QJsonValue value;

	// Check version...

	value = root.value("Version");
	if (!value.isString())
		return false;

	QString version = value.toString();
	QStringList versionElements = version.split('.');
	if (versionElements.size() < 2)
		return false;

	m_versionMajor = versionElements.at(0).toInt();
	m_versionMinor = versionElements.at(1).toInt();
	if (m_versionMajor > SGE_VERSION_MAJOR || m_versionMinor > SGE_VERSION_MINOR)
		return false;

	// Load name...

	value = root.value("Name");
	if (value.isString())
		m_name = value.toString();
	else
		m_name = QString();

	// Load assets...
	
	value = root.value("Assets");
	if (!value.isArray())
		return false;

	QJsonArray assets = value.toArray();

	// Load scenes...

	value = root.value("Scenes");
	if (!value.isArray())
		return false;

	QJsonArray scenes = value.toArray();

	// Load Levels...

	value = root.value("Levels");
	if (!value.isArray())
		return false;

	QJsonArray levels = value.toArray();

	return true;
}

bool Game::save(FileSystem *fs)
{
	Q_ASSERT(fs != Q_NULLPTR);

	if (fs->isReadonly())
		return false;

	QJsonObject root;

	root["Version"] = QString("%1.%2").arg(SGE_VERSION_MAJOR).arg(SGE_VERSION_MINOR);
	root["Name"] = m_name.isNull() ? "Untitled" : m_name;

	// TODO game data to json.

	QJsonDocument manifest(root);

	FilePtr file = fs->openManifest(QIODevice::WriteOnly | QIODevice::Truncate);
	if (!file)
		return false;

	QByteArray data = manifest.toJson();
	if (data.isNull() || data.isEmpty())
		return false;

	qint64 size = file->write(data);
	if (size != data.size())
		return false;

	return true;
}

void Game::update(float elapsed)
{
	switch (m_state) {
	case StateIdle:
		break;
	}
}

void Game::draw(Renderer::View *view)
{
	Q_ASSERT(view != Q_NULLPTR);

	switch (m_state) {
	case StateIdle:
		break;
	}
}

SGE_END
