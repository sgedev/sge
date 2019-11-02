//
//
#include <QMutexLocker>

#include <sge/client.hpp>

SGE_BEGIN

Client::Client(QObject *parent)
	: QObject(parent)
	, m_root(NULL)
{
}

Client::~Client(void)
{
}

bool Client::init(ttvfs::Root *root)
{
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

