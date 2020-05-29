//
//
#include <SGE/Graphics/Heap.hpp>

SGE_GRAPHICS_BEGIN

static const int HeapBlockOrder = 26;
static const int HeapBlockSize = (1 << HeapBlockOrder);

struct Heap::Buffer {
    cx_list_node_t node;
	Heap::Block *block;
	int pos;
	int size;
};

struct Heap::Block {
    cx_list_node_t node;
    cx_list_t bufferList;
    Heap *heap;
    int freeSize;
    GLuint id;
};

// Heap

Heap::Heap(GLenum target, GLenum usage):
    m_target(target),
	m_usage(usage)
{
    cx_list_reset(&m_blockList);
}

Heap::~Heap(void)
{
    cx_list_node_t *node;
    Block *block;

    while (!cx_list_empty(&m_blockList)) {
        node = cx_list_del_head(&m_blockList);
        block = CX_MEMBEROF(node, Block, node);
        destroyBlock(block);
    }
}

Heap::Buffer *Heap::allocBuffer(int size)
{
    SGE_ASSERT(size > 0);

    if (size > HeapBlockSize)
        return NULL;

    cx_list_node_t *node;
    Block *block;
    Buffer *buf;

    CX_LIST_FOREACH(node, &m_blockList) {
        block = CX_MEMBEROF(node, Block, node);
        buf = allocBufferFromBlock(block, size);
        if (buf != NULL)
            return buf;
    }

    block = createBlock();
    if (block == NULL)
        return NULL;

    return allocBufferFromBlock(block, size);
}

void Heap::freeBuffer(Buffer *buf)
{
    SGE_ASSERT(buf != NULL);

    cx_list_node_unlink(&buf->node);
    delete buf;
}

int Heap::bufferSize(Buffer *buf)
{
    SGE_ASSERT(buf != NULL);

    return buf->size;
}

int Heap::readBuffer(Buffer *buf, int offset, void *p, int size)
{
    SGE_ASSERT(buf != NULL);

    return 0;
}

int Heap::writeBuffer(Buffer *buf, int offset, const void *p, int size)
{
    return 0;
}

Heap::Block *Heap::createBlock(void)
{
    Block *block = new Block();

    glGenBuffers(1, &block->id);
    if (block->id == 0) {
        delete block;
        return NULL;
    }

    glBindBuffer(m_target, block->id);
    glBufferData(m_target, HeapBlockSize, NULL, m_usage);
    glBindBuffer(m_target, 0);

    block->heap = this;
    block->freeSize = HeapBlockSize;

    cx_list_reset(&block->bufferList);
    cx_list_node_reset(&block->node);
    cx_list_add_head(&m_blockList, &block->node);

    return block;
}

void Heap::destroyBlock(Block *block)
{
    SGE_ASSERT(block != NULL);
    SGE_ASSERT(cx_list_empty(&block->bufferList));

    cx_list_del_node(&m_blockList, &block->node);

    delete block;
}

Heap::Buffer *Heap::allocBufferFromBlock(Block *block, int size)
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
        if (used != NULL && size > (HeapBlockSize - pos))
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
