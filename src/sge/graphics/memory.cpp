//
//
#include <sge/graphics/memory.hpp>

SGE_GRAPHICS_BEGIN

memory::memory(void)
{
}

memory::~memory(void)
{
}

buffer *memory::alloc_static_vertices(int count)
{
    return alloc_buffer(m_static_vertex_heaps, sizeof(vertex_t) * count,
        GL_ARRAY_BUFFER, GL_STATIC_DRAW);
}

buffer *memory::alloc_static_vertex_indices(int count)
{
    return alloc_buffer(m_static_vertex_index_heaps, sizeof(vertex_index_t) * count,
        GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
}

buffer *memory::alloc_dynamic_vertices(int count)
{
    return alloc_buffer(m_dynamic_vertex_heaps, sizeof(vertex_t) * count,
        GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
}

buffer *memory::alloc_dynamic_vertex_indices(int count)
{
    return alloc_buffer(m_dynamic_vertex_index_heaps, sizeof(vertex_index_t) * count,
        GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
}

buffer *memory::alloc_buffer(heap_list_t &lp, GLsizeiptr size, GLenum target, GLenum usage)
{
    SGE_ASSERT(size > 0);

    buffer *buf;

    for (auto it(lp.begin()); it != lp.end(); ++it) {
        buf = (*it)->alloc_buffer(size);
        if (buf != nullptr)
            return buf;
    }

    heap *ph = new heap(target, usage);
    if (ph == nullptr)
        return nullptr;

    if (!ph->init()) {
        delete ph;
        return nullptr;
    }

    buf = ph->alloc_buffer(size);
    lp.push_front(ph);

    return buf;
}

SGE_GRAPHICS_END
