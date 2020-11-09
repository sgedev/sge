//
//
#ifndef SGE_GRAPHICS_MEMORY_HPP
#define SGE_GRAPHICS_MEMORY_HPP

#include <list>

#include <sge/graphics/common.hpp>
#include <sge/graphics/buffer.hpp>
#include <sge/graphics/heap.hpp>

SGE_GRAPHICS_BEGIN

class memory {
public:
    memory(void);
    virtual ~memory(void);

public:
    buffer *alloc_static_vertices(int count);
    buffer *alloc_static_vertex_indices(int count);
    buffer *alloc_dynamic_vertices(int count);
    buffer *alloc_dynamic_vertex_indices(int count);

private:
    typedef std::list<heap *> heap_list_t;

    buffer *alloc_buffer(heap_list_t &lp, GLsizeiptr size, GLenum target, GLenum usage);

private:
    heap_list_t m_static_vertex_heaps;
    heap_list_t m_static_vertex_index_heaps;
    heap_list_t m_dynamic_vertex_heaps;
    heap_list_t m_dynamic_vertex_index_heaps;
};

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_MEMORY_HPP
