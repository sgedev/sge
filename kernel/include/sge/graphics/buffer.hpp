//
//
#ifndef SGE_GRAPHICS_BUFFER_HPP
#define SGE_GRAPHICS_BUFFER_HPP

#include <sge/io.hpp>
#include <sge/graphics/common.hpp>

SGE_GRAPHICS_BEGIN

class buffer: public io {
public:
    buffer(void);
    ~buffer(void) override;

public:
    virtual void bind(void) = 0;
};

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_BUFFER_HPP
