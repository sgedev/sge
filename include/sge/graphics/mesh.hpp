//
//
#ifndef SGE_GRAPHICS_MESH_HPP
#define SGE_GRAPHICS_MESH_HPP

#include <sge/graphics/common.hpp>
#include <sge/graphics/buffer.hpp>
#include <sge/graphics/memory.hpp>

SGE_GRAPHICS_BEGIN

class mesh {
public:
    enum type {
        TYPE_STATIC = 0,
        TYPE_DYNAMIC
    };

public:
    mesh(memory &m);
    virtual ~mesh(void);

public:
    bool init(int vertex_count, int vertex_index_count, type t);
    int vertex_count(void) const;
    int vertex_index_count(void) const;
    void bind(void);

private:
    memory &m_memory;
    buffer *m_vertex_buffer;
    buffer *m_vertex_index_buffer;
    int m_vertex_count;
    int m_vertex_index_count;
};

SGE_INLINE int mesh::vertex_count(void) const
{
    return m_vertex_count;
}

SGE_INLINE int mesh::vertex_index_count(void) const
{
    return m_vertex_index_count;
}

SGE_INLINE void mesh::bind(void)
{
    SGE_ASSERT(m_vertex_buffer != nullptr);
    SGE_ASSERT(m_vertex_index_buffer != nullptr);

    m_vertex_buffer->bind();
    m_vertex_index_buffer->bind();
}

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_MESH_HPP
