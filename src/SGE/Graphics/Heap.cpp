//
//
#include <SGE/Graphics/Heap.hpp>

SGE_GRAPHICS_BEGIN

static const int HeapBlockOrder = 26;
static const int HeapBlockSize = (1 << HeapBlockOrder);

struct Heap::Buffer {
    Buffer(void):
        node(this)
    {
    }

	Heap::Block *block;
	int pos;
	int size;
    List<Buffer>::Node node;
};

struct Heap::Block {
    Block(void):
        node(this)
    {
    }

    Heap *heap;
    int freeSize;
    GLuint id;
    List<Block>::Node node;
    List<Buffer> bufferList;
};

// Heap

Heap::Heap(GLenum target, GLenum usage):
    m_target(target),
	m_usage(usage)
{
}

Heap::~Heap(void)
{
    List<Block>::Node *p;
    for (p = m_blockList.first(); p != m_blockList.knot(); p = p->next())
        destroyBlock(p->value());
}

Heap::Buffer *Heap::allocBuffer(int size)
{
    Block *blk;
    Buffer *buf;

    SGE_ASSERT(size > 0);

    if (size > HeapBlockSize)
        return NULL;

    for (List<Block>::Node *p = m_blockList.first(); p != m_blockList.knot(); p = p->next()) {
        buf = allocBufferFromBlock(p->value(), size);
        if (buf != NULL)
            return buf;
    }

    blk = createBlock();
    if (blk == NULL)
        return NULL;

    return allocBufferFromBlock(blk, size);
}

void Heap::freeBuffer(Buffer *buf)
{
    SGE_ASSERT(buf != NULL);

    buf->node.unlink();
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
    Block *blk = new Block();

    glGenBuffers(1, &blk->id);
    if (blk->id == 0) {
        delete blk;
        return NULL;
    }

    glBindBuffer(m_target, blk->id);
    glBufferData(m_target, HeapBlockSize, NULL, m_usage);
    glBindBuffer(m_target, 0);

    blk->heap = this;
    blk->freeSize = HeapBlockSize;

    m_blockList.prepand(&blk->node);

    return blk;
}

void Heap::destroyBlock(Block *blk)
{
    SGE_ASSERT(blk != NULL);
    SGE_ASSERT(blk->bufferList.empty());

    m_blockList.remove(&blk->node);
    delete blk;
}

Heap::Buffer *Heap::allocBufferFromBlock(Block *blk, int size)
{
    int pos = 0;
    List<Buffer>::Node *used;
    Buffer *buf;
    bool found = false;

    SGE_ASSERT(size > 0);

    if (size > blk->freeSize)
        return NULL;

    for (used = blk->bufferList.first(); used != blk->bufferList.knot(); used = used->next()) {
        if ((used->value()->pos - pos) >= size) {
            found = true;
            break;
        }
        pos = used->value()->pos + used->value()->size;
    }

    buf = new Buffer();
    if (!found) {
        if (used != NULL && size > (HeapBlockSize - pos))
            return NULL;
        blk->bufferList.append(&buf->node);
    } else
        buf->node.link(used, used->next());

    buf->block = blk;
    buf->pos = pos;
    buf->size = size;

    blk->freeSize -= size;

    return 0;
}

SGE_GRAPHICS_END
