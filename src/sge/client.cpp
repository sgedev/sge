//
//
#include <sge/client.hpp>

SGE_BEGIN

Client::Client(QObject *parent)
	: QObject(parent)
	, m_db(Q_NULLPTR)
{
}

Client::~Client(void)
{
}

bool Client::init(Database::Connection *db)
{
	Q_ASSERT(db != Q_NULLPTR);
	Q_ASSERT(m_db == Q_NULLPTR);

	m_db = db;

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

