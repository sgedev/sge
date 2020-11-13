//
//
#ifndef SGE_GRAPHICS_FRAME_BUFFER_HPP
#define SGE_GRAPHICS_FRAME_BUFFER_HPP

#include <sge/graphics/common.hpp>
#include <sge/graphics/surface.hpp>

SGE_GRAPHICS_BEGIN

class frame_buffer: public surface {
public:
	frame_buffer(void);
    ~frame_buffer(void) override;

public:
    GLuint id(void) const;
	void resize(int width, int height) override;

private:
    GLuint m_id;
};

SGE_INLINE GLuint frame_buffer::id(void) const
{
    return m_id;
}

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_FRAME_BUFFER_HPP
