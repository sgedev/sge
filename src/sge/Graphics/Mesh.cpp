//
//
#include <SGE/Graphics/Mesh.hpp>

SGE_GRAPHICS_BEGIN

Mesh::Mesh(Memory &memory) :
	m_memory(memory),
	m_vertexBuffer(NULL),
	m_vertexCount(0),
	m_vertexIndexBuffer(NULL),
	m_vertexIndexCount(0)
{
}

Mesh::~Mesh(void)
{
	if (m_vertexBuffer != NULL)
		m_memory.freeBuffer(m_vertexBuffer);
	if (m_vertexIndexBuffer != NULL)
		m_memory.freeBuffer(m_vertexIndexBuffer);
}

bool Mesh::init(int vertexCount, int vertexIndexCount)
{
	SGE_ASSERT(vertexCount > 0);
	SGE_ASSERT(vertexIndexCount > 0);
	SGE_ASSERT(m_vertexBuffer == NULL);
	SGE_ASSERT(m_vertexCount == 0);
	SGE_ASSERT(m_vertexIndexBuffer == NULL);
	SGE_ASSERT(m_vertexIndexCount == 0);

	m_vertexBuffer = m_memory.allocBuffer(sizeof(Vertex) * vertexCount, Memory::TypeStaticVertex);
	m_vertexIndexBuffer = m_memory.allocBuffer(sizeof(VertexIndex) * vertexIndexCount, Memory::TypeStaticVertexIndex);
	m_vertexCount = vertexCount;
	m_vertexIndexCount = vertexIndexCount;

	return true;
}

SGE_GRAPHICS_END
