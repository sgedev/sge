//
//
#ifndef SGE_VM_NODE_HPP
#define SGE_VM_NODE_HPP

#include <list>

#include <sge/object.hpp>
#include <sge/graphics/view.hpp>
#include <sge/vm/common.hpp>

SGE_VM_BEGIN

class node;
typedef std::list<node *> node_list;

class node: public object {
public:
    node(void);
    ~node(void) override;

public:
    bool is_enable(void) const;
    void set_enable(bool v);
    bool is_visible(void) const;
    void set_visible(bool v);
    node *parent(void);
    void set_parent(node *p);
    const node_list &children(void) const;
    virtual void handle_event(const SDL_Event &evt);
    virtual void update(float elapsed);
    virtual void add_to_view(graphics::view &v);
    static void init_exports(lua_State *L);

protected:
    static int do_trap(lua_State *L, lua_CFunction func);

private:
    enum {
        FLAG_ENABLE = 0x1,
        FLAG_VISIBLE = 0x2
    };

    int m_flags;
    node *m_parent;
    node_list m_children;
};

SGE_INLINE bool node::is_enable(void) const
{
    return m_flags & FLAG_ENABLE;
}

SGE_INLINE void node::set_enable(bool v)
{
    if (v)
        m_flags |= FLAG_ENABLE;
    else
        m_flags &= ~FLAG_ENABLE;
}

SGE_INLINE bool node::is_visible(void) const
{
    return m_flags & FLAG_VISIBLE;
}

SGE_INLINE void node::set_visible(bool v)
{
    if (v)
        m_flags |= FLAG_VISIBLE;
    else
        m_flags &= ~FLAG_VISIBLE;
}


SGE_INLINE node *node::parent(void)
{
    return m_parent;
}

SGE_INLINE const node_list &node::children(void) const
{
    return m_children;
}

SGE_VM_END

#endif // SGE_VM_NODE_HPP
