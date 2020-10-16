//
//
#include <sge/graphics/mesh.hpp>

SGE_GRAPHICS_BEGIN

mesh::mesh(memory &m):
    m_memory(m),
    m_vertex_buffer(nullptr),
    m_vertex_index_buffer(nullptr),
    m_vertex_count(0),
    m_vertex_index_count(0)
{
}

mesh::~mesh(void)
{
}

bool mesh::init(int vertex_count, int vertex_index_count, type t)
{
    SGE_ASSERT(m_vertex_buffer == nullptr);
    SGE_ASSERT(m_vertex_index_buffer == nullptr);

    switch (t) {
    case TYPE_STATIC:
        m_vertex_buffer = m_memory.alloc_static_vertices(vertex_count);
        if (m_vertex_buffer == nullptr)
            return false;
        m_vertex_index_buffer = m_memory.alloc_static_vertex_indices(vertex_index_count);
        if (m_vertex_index_buffer == nullptr) {
            delete m_vertex_buffer;
            return false;
        }
        break;
    case TYPE_DYNAMIC:
        m_vertex_buffer = m_memory.alloc_dynamic_vertices(vertex_count);
        if (m_vertex_buffer == nullptr)
            return false;
        m_vertex_index_buffer = m_memory.alloc_dynamic_vertex_indices(vertex_index_count);
        if (m_vertex_index_buffer == nullptr) {
            delete m_vertex_buffer;
            return false;
        }
        break;
    default:
        return false;
    }

    m_vertex_count = vertex_count;
    m_vertex_index_count = vertex_index_count;

    return true;
}

SGE_GRAPHICS_END
