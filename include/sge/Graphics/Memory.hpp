//
//
#ifndef SGE_GRAPHICS_MEMORY_HPP
#define SGE_GRAPHICS_MEMORY_HPP

#include <cx/list.h>

#include <SGE/Graphics/Common.hpp>

SGE_GRAPHICS_BEGIN

class Memory {
public:
    struct Buffer;

public:
    Memory(void);
    virtual ~Memory(void);

public:
    enum Type {
        TypeStaticVertex = 0,
        TypeStaticVertexIndex,
        TypeDynamicVertex,
        TypeDynamicVertexIndex,
        TypeMax
    };

    Buffer *allocBuffer(int size, Type type);
    void freeBuffer(Buffer *buf);
    int bufferSize(Buffer *buf);
    int readBuffer(Buffer *buf, int offset, void *p, int size);
    int writeBuffer(Buffer *buf, int offset, const void *p, int size);

private:
    struct Block;
    static GLenum target(Type type);
    static GLenum usage(Type type);
    cx_list_t *blockList(Type type);
    Block *createBlock(Type type);
    void destroyBlock(Block *blk);
    Buffer *allocBufferFromBlock(Block *blk, int size);

private:
    cx_list_t m_blockList[TypeMax];
};

SGE_INLINE GLenum Memory::target(Type type)
{
    SGE_ASSERT(0 <= type && type < TypeMax);

    static const GLenum m[TypeMax] = {
        GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER,
        GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER
    };

    return m[type];
}

SGE_INLINE GLenum Memory::usage(Type type)
{
    SGE_ASSERT(0 <= type && type < TypeMax);

    static const GLenum m[TypeMax] = {
        GL_STATIC_DRAW, GL_STATIC_DRAW,
        GL_DYNAMIC_DRAW, GL_DYNAMIC_DRAW
    };

    return m[type];
}

SGE_INLINE cx_list_t *Memory::blockList(Type type)
{
    SGE_ASSERT(0 <= type && type < TypeMax);

    return &m_blockList[type];
}

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_MEMORY_HPP
