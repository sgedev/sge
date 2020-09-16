//
//
#include <sge/renderer/light.hpp>

SGE_RENDERER_BEGIN

light::light(void):
    m_color(1.0f, 1.0f, 1.0f),
    m_strength(1.0f)
{
}

light::~light(void)
{
}

SGE_RENDERER_END
