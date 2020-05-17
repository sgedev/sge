//
//
#ifndef SGE_GRAPHICS_HEAP_HPP
#define SGE_GRAPHICS_HEAP_HPP

#include <SGE/List.hpp>
#include <SGE/Graphics/Common.hpp>

SGE_GRAPHICS_BEGIN

class Buffer;
class Chunk;

class Heap {
public:
    struct Buffer;

public:
    Heap(GLenum target, GLenum usage);
    virtual ~Heap(void);

public:
    Buffer *allocBuffer(int size);
    void freeBuffer(Buffer *buf);
    int bufferSize(Buffer *buf);
    int readBuffer(Buffer *buf, int offset, void *p, int size);
    int writeBuffer(Buffer *buf, int offset, const void *p, int size);

private:
    struct Block;
    Block *createBlock(void);
    void destroyBlock(Block *blk);
    Buffer *allocBufferFromBlock(Block *blk, int size);

private:
    GLenum m_target;
    GLenum m_usage;
    List<Block> m_blockList;
};

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_HEAP_HPP
