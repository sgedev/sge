//
//
#include <algorithm>
#include <functional>

#include <sge/graphics/heap.hpp>

SGE_GRAPHICS_BEGIN

// heap_buffer

class heap_buffer: public buffer {
public:
    std::function<void (void)> free_function;
    std::function<void (void)> bind_function;
    std::function<void *(void)> map_function;
    std::function<void (void)> unmap_function;

public:
    heap_buffer(GLintptr offset, GLsizeiptr size);
    ~heap_buffer(void) override;

public:
    bool open(int flags) override;
    void close(void) override;
    int64_t size(void) override;
    int64_t read(void *p, int64_t size) override;
    int64_t write(const void *p, int64_t size) override;
    void bind(void) override;
    GLintptr offset(void) const;

protected:
    int64_t pos(void) override;
    int64_t set_pos(int64_t pos) override;

private:
    GLintptr m_offset;
    GLsizeiptr m_size;
    void *m_map;
    int m_pos;
};

heap_buffer::heap_buffer(GLintptr offset, GLsizeiptr size):
    m_offset(offset),
    m_size(size),
    m_map(nullptr),
    m_pos(-1)
{
}

heap_buffer::~heap_buffer(void)
{
    if (is_open())
        close();
}

bool heap_buffer::open(int flags)
{
    SGE_ASSERT(m_map == nullptr);

    if (!buffer::open(flags))
        return false;

    m_map = map_function();
    if (m_map == nullptr)
        return false;

    set_seekable(true);
    m_pos = 0;

    return true;
}

void heap_buffer::close(void)
{
    SGE_ASSERT(m_map != nullptr);

    unmap_function();
    m_map = nullptr;
}

int64_t heap_buffer::size(void)
{
    return m_size;
}

int64_t heap_buffer::read(void *p, int64_t size)
{
    SGE_ASSERT(m_map != nullptr);
    SGE_ASSERT(is_readable());

    int64_t len = std::min(m_size - m_offset - m_pos, size);
    if (len > 0) {
        memcpy(p, ((uint8_t *)m_map) + m_offset + m_pos, len);
        m_pos += len;
    }

    return len;
}

int64_t heap_buffer::write(const void *p, int64_t size)
{
    SGE_ASSERT(m_map != nullptr);
    SGE_ASSERT(is_writable());

    int64_t len = std::min(m_size - -m_offset - m_pos, size);
    if (len > 0) {
        memcpy(((uint8_t *)m_map) + m_offset + m_pos, p, len);
        m_pos += len;
    }

    return len;
}

void heap_buffer::bind(void)
{
    bind_function();
}

int64_t heap_buffer::pos(void)
{
    SGE_ASSERT(m_map != nullptr);
    SGE_ASSERT(is_seekable());

    return m_pos;
}

int64_t heap_buffer::set_pos(int64_t pos)
{
    SGE_ASSERT(m_map != nullptr);
    SGE_ASSERT(is_seekable());

    m_pos = std::clamp(pos, int64_t(0), m_size);

    return m_pos;
}

SGE_INLINE GLintptr heap_buffer::offset(void) const
{
    return m_offset;
}

// heap

heap::heap(GLenum target, GLenum usage):
    m_target(target),
    m_usage(usage),
    m_buffer(0),
    m_size(0),
    m_free_size(0),
    m_map(nullptr),
    m_map_count(0)
{
}

heap::~heap(void)
{
    SGE_ASSERT(m_buffer == 0);
    SGE_ASSERT(m_map == nullptr);
    SGE_ASSERT(m_map_count == 0);
}

bool heap::init(int order)
{
    SGE_ASSERT(order >= 0);
    SGE_ASSERT(m_buffer == 0);

    glGenBuffers(1, &m_buffer);
    if (m_buffer == 0)
        return false;

    GLsizeiptr size = 1 << order;

    glBindBuffer(m_target, m_buffer);
    glBufferData(m_target, size, nullptr, m_usage);

    m_size = size;
    m_free_size = size;

    return true;
}

void heap::release(void)
{
    SGE_ASSERT(m_buffer_list.empty());
}

buffer *heap::alloc_buffer(GLsizeiptr size)
{
    SGE_ASSERT(size > 0);
    SGE_ASSERT(m_buffer > 0);

    if (size > m_free_size)
        return nullptr;

    buffer_list_t::iterator it;
    heap_buffer *used;
    int offset = 0;

    for (it = m_buffer_list.begin(); it != m_buffer_list.end(); ++it) {
        used = static_cast<heap_buffer *>(*it);
        if ((used->offset() - offset) >= size)
            break;
        offset = used->offset() + used->size();
    }

    heap_buffer *buf = new heap_buffer(offset, size);
    if (buf == nullptr)
        return nullptr;

    if (it == m_buffer_list.end()) {
        if (used != nullptr && size > (m_size - offset))
            return nullptr;
        m_buffer_list.push_front(buf);
    } else
        m_buffer_list.insert(it, buf);

    m_free_size -= size;

    buf->free_function = std::bind(&heap::free_buffer, this, buf);
    buf->bind_function = std::bind(&heap::bind_buffer, this, buf);
    buf->map_function = std::bind(&heap::map_buffer, this, buf);
    buf->unmap_function = std::bind(&heap::unmap_buffer, this, buf);

    return buf;
}

void heap::free_buffer(buffer *buf)
{
    m_buffer_list.remove(buf);
}

void heap::bind_buffer(buffer *buf)
{
    SGE_UNUSED(buf);

    glBindBuffer(m_target, m_buffer);
}

void *heap::map_buffer(buffer *buf)
{
    SGE_UNUSED(buf);

    if (m_map == nullptr)
    {
        SGE_ASSERT(m_map_count == 0);
        glBindBuffer(m_target, m_buffer);
        m_map = glMapBuffer(m_target, GL_READ_WRITE);
        if (m_map == nullptr)
            return nullptr;
    }

    m_map_count += 1;

    return m_map;
}

void heap::unmap_buffer(buffer *buf)
{
    SGE_ASSERT(m_map != nullptr);
    SGE_ASSERT(m_map_count > 0);
    SGE_UNUSED(buf);

    m_map_count -= 1;
    if (m_map_count > 0)
        return;

    glBindBuffer(m_target, m_buffer);
    glUnmapBuffer(m_target);
    m_map = nullptr;
}

SGE_GRAPHICS_END
