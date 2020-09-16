//
//
#ifndef SGE_RENDERER_FB_HPP
#define SGE_RENDERER_FB_HPP

#include <sge/renderer/common.hpp>

SGE_RENDERER_BEGIN

class fb {
public:
    fb(void);
    virtual ~fb(void);

public:
    GLuint id(void) const;
    int width(void) const;
    void set_width(int v);
    int height(void) const;
    void set_height(int v);
    void resize(int w, int h);

private:
    GLuint m_id;
    int m_width;
    int m_height;
};

SGE_INLINE GLuint fb::id(void) const
{
    return m_id;
}

SGE_INLINE int fb::width(void) const
{
    return m_width;
}

SGE_INLINE void fb::set_width(int v)
{
    SGE_ASSERT(v > 0);
    m_width = v;
}

SGE_INLINE int fb::height(void) const
{
    return m_height;
}

SGE_INLINE void fb::set_height(int v)
{
    SGE_ASSERT(v > 0);
    m_height = v;
}

SGE_INLINE void fb::resize(int w, int h)
{
    set_width(w);
    set_height(h);
}

SGE_RENDERER_END

#endif // SGE_RENDERER_FB_HPP
