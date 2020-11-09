//
//
#ifndef SGE_OBJECT_HPP
#define SGE_OBJECT_HPP

#include <string>

#include <sge/list.h>
#include <sge/common.hpp>

SGE_BEGIN

class object {
public:
    object(void);
    virtual ~object(void);

public:
    const std::string &name(void) const;
    void set_name(const std::string &r);

private:
    std::string m_name;

    RTTR_ENABLE();
};

SGE_INLINE const std::string &object::name(void) const
{
    return m_name;
}

SGE_INLINE void object::set_name(const std::string &name)
{
    m_name = name;
}

SGE_END

#endif // SGE_OBJECT_HPP
