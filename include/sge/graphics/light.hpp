//
//
#ifndef SGE_GRAPHICS_LIGHT_HPP
#define SGE_GRAPHICS_LIGHT_HPP

#include <sge/graphics/common.hpp>

SGE_GRAPHICS_BEGIN

class light {
    RTTR_ENABLE()

public:
    light(void);
    virtual ~light(void);

public:
    const glm::vec3 &color(void) const;
    void set_color(const glm::vec3 &r);
    float strength(void) const;
    void set_strength(float v);

private:
    glm::vec3 m_color;
    float m_strength;
};

SGE_INLINE const glm::vec3 &light::color(void) const
{
    return m_color;
}

SGE_INLINE void light::set_color(const glm::vec3 &r)
{
    m_color = r;
}

SGE_INLINE float light::strength(void) const
{
    return m_strength;
}

SGE_INLINE void light::set_strength(float v)
{
    m_strength = v;
}

SGE_GRAPHICS_END

#endif // SGE_GRAPHICS_LIGHT_HPP
