//
//
#ifndef SGE_GRAPHICS_HEAP_HPP
#define SGE_GRAPHICS_HEAP_HPP

#include <list>

#include <sge/graphics/common.hpp>
#include <sge/graphics/buffer.hpp>

SGE_GRAPHICS_BEGIN

class heap {
public:
    heap(GLenum target, GLenum usage);
    virtual ~heap(void);

public:
    bool init(int order = 26);
    void release(void);
    GLenum target(void) const;
    GLenum usage(void) const;
    GLsizeiptr size(void) const;
    GLsizeiptr free_size(void) const;
    buffer *alloc_buffer(GLsizeiptr size);

protected:
    void free_buffer(buffer *buf);
    void bind_buffer(buffer *buf);
    void *map_buffer(buffer *buf);
    void unmap_buffer(buffer *buf);

private:
    typedef std::list<buffer *> buffer_list_t;

private:
    GLenum m_target;
    GLenum m_usage;
    GLuint m_buffer;
    GLsizeiptr m_size;
    GLsizeiptr m_free_size;
    void *m_map;
    int m_map_count;
    buffer_list_t m_buffer_list;
};

SGE_INLINE GLenum heap::target(void) const
{
    return m_target;
}

SGE_INLINE GLenum heap::usage(void) const
{
    return m_usage;
}

SGE_INLINE GLsizeiptr heap::size(void) const
{
    return m_size;
}

SGE_INLINE GLsizeiptr heap::free_size(void) const
{
    return m_free_size;
}

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_HEAP_HPP
