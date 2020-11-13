//
//
#ifndef SGE_VM_NODE_HPP
#define SGE_VM_NODE_HPP

#include <string>

#include <sge/vm/common.hpp>

SGE_VM_BEGIN

class node {
public:
    node(void);
    virtual ~node(void);

public:
    const std::string &name(void) const;
    void rename(const std::string &name);
    virtual void update(float elapsed);

private:
    std::string m_name;
};

SGE_INLINE const std::string &node::name(void) const
{
    return m_name;
}

SGE_INLINE void node::rename(const std::string &name)
{
    m_name = name;
}

SGE_VM_END

#endif // SGE_VM_NODE_HPP
