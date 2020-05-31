//
//
#include <SGE/Graphics/Memory.hpp>

SGE_GRAPHICS_BEGIN

static const int MemoryBlockOrder = 26;
static const int MemoryBlockSize = (1 << MemoryBlockOrder);

struct Memory::Buffer {
    cx_list_node_t node;
	Memory::Block *block;
	int pos;
	int size;
};

struct Memory::Block {
    cx_list_node_t node;
    cx_list_t bufferList;
    GLenum target;
    GLenum usage;
    GLuint id;
    Memory *heap;
    int freeSize;
};

// Heap

Memory::Memory(void)
{
    for (int i = 0; i < TypeMax; ++i)
        cx_list_reset(&m_blockList[i]);
}

Memory::~Memory(void)
{
    cx_list_node_t *node;
    Block *block;

    for (int i = 0; i < TypeMax; ++i) {
        while (!cx_list_empty(&m_blockList[i])) {
            node = cx_list_del_head(&m_blockList[i]);
            block = CX_MEMBEROF(node, Block, node);
            destroyBlock(block);
        }
    }
}

Memory::Buffer *Memory::allocBuffer(int size, Type type)
{
    SGE_ASSERT(size > 0);

    if (size > MemoryBlockSize)
        return NULL;

    cx_list_t *list = blockList(type);
    SGE_ASSERT(list != NULL);

    cx_list_node_t *node;
    Block *block;
    Buffer *buf;

    CX_LIST_FOREACH(node, list) {
        block = CX_MEMBEROF(node, Block, node);
        buf = allocBufferFromBlock(block, size);
        if (buf != NULL)
            return buf;
    }

    block = createBlock(type);
    if (block == NULL)
        return NULL;

    return allocBufferFromBlock(block, size);
}

void Memory::freeBuffer(Buffer *buf)
{
    SGE_ASSERT(buf != NULL);

    cx_list_node_unlink(&buf->node);
    delete buf;
}

int Memory::bufferSize(Buffer *buf)
{
    SGE_ASSERT(buf != NULL);

    return buf->size;
}

int Memory::readBuffer(Buffer *buf, int offset, void *p, int size)
{
    SGE_ASSERT(buf != NULL);

    return 0;
}

int Memory::writeBuffer(Buffer *buf, int offset, const void *p, int size)
{
    return 0;
}

Memory::Block *Memory::createBlock(Type type)
{
    Block *block = new Block();

    glGenBuffers(1, &block->id);
    if (block->id == 0) {
        delete block;
        return NULL;
    }

    GLenum ta = target(type);
    GLenum ua = usage(type);

    glBindBuffer(ta, block->id);
    glBufferData(ta, MemoryBlockSize, NULL, ua);
    glBindBuffer(ta, 0);

    block->heap = this;
    block->freeSize = MemoryBlockSize;
    block->target = ta;
    block->usage = ua;

    cx_list_t *list = blockList(type);
    SGE_ASSERT(list != NULL);

    cx_list_reset(&block->bufferList);
    cx_list_node_reset(&block->node);
    cx_list_add_head(list, &block->node);

    return block;
}

void Memory::destroyBlock(Block *block)
{
    SGE_ASSERT(block != NULL);
    SGE_ASSERT(cx_list_empty(&block->bufferList));

    cx_list_node_unlink(&block->node);

    delete block;
}

Memory::Buffer *Memory::allocBufferFromBlock(Block *block, int size)
{
    int pos = 0;
    cx_list_node_t *node;
    Buffer *used;
    Buffer *buf;
    bool found = false;

    SGE_ASSERT(size > 0);

    if (size > block->freeSize)
        return NULL;

    CX_LIST_FOREACH(node, &block->bufferList) {
        used = CX_MEMBEROF(node, Buffer, node);
        if ((used->pos - pos) >= size) {
            found = true;
            break;
        }
        pos = used->pos + used->size;
    }

    buf = new Buffer();
    cx_list_node_reset(&buf->node);

    if (!found) {
        if (used != NULL && size > (MemoryBlockSize - pos))
            return NULL;
        cx_list_add_tail(&block->bufferList, &buf->node);
    } else
        cx_list_node_link(&buf->node, node, cx_list_node_next(node));

    buf->block = block;
    buf->pos = pos;
    buf->size = size;

    block->freeSize -= size;

    return 0;
}

SGE_GRAPHICS_END
