//
//
#include <sge/vm/node.hpp>

SGE_VM_BEGIN

node::node(void):
    m_parent(nullptr)
{
}

node::~node(void)
{
    set_parent(nullptr);
}

void node::set_parent(node *p)
{
    if (m_parent != nullptr)
        m_parent->m_children.remove(this);

    m_parent = p;

    if (m_parent != nullptr)
        m_parent->m_children.push_back(this);
}

void node::handle_event(const SDL_Event &evt)
{
}

void node::update(float elapsed)
{
}

void node::add_to_view(graphics::view &v)
{
}

SGE_VM_END
