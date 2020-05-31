//
//
#ifndef SGE_GRAPHICS_MESH_HPP
#define SGE_GRAPHICS_MESH_HPP

#include <SGE/Graphics/Common.hpp>
#include <SGE/Graphics/Memory.hpp>

SGE_GRAPHICS_BEGIN

class Mesh {
public:
	Mesh(Memory &memory);
	virtual ~Mesh(void);

public:
	bool init(int vertexCount, int vertexIndexCount);

private:
	Memory &m_memory;
	Memory::Buffer *m_vertexBuffer;
	int m_vertexCount;
	Memory::Buffer *m_vertexIndexBuffer;
	int m_vertexIndexCount;
};

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_MESH_HPP
