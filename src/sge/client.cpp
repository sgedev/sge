//
//
#include <QMutexLocker>

#include <sge/client.hpp>

SGE_BEGIN

Client::Client(QObject *parent)
	: QObject(parent)
	, m_fs(Q_NULLPTR)
{
}

Client::~Client(void)
{
}

bool Client::init(FileSystem *fs)
{
	Q_ASSERT(m_fs == Q_NULLPTR);
	Q_ASSERT(fs != Q_NULLPTR);

	m_fs = fs;

	return true;
}

void Client::shutdown(void)
{
}

void Client::update(float elapsed)
{
}

void Client::draw(Renderer::View *view)
{
}

SGE_END

