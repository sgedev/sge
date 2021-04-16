//
//
#ifndef SGE_SCOPE_GUARD_HPP
#define SGE_SCOPE_GUARD_HPP

#include <functional>

#include <sge/common.hpp>

SGE_BEGIN

class scope_guard final {
public:
	using function = std::function<void (void)>;

public:
	scope_guard(function &fn);
	scope_guard(function &&fn);
	~scope_guard(void);

public:
	void dismiss(void);

private:
	function m_fn;
	bool m_dismissed;
};

SGE_INLINE scope_guard::scope_guard(function &fn):
	m_fn(fn),
	m_dismissed(false)
{
}

SGE_INLINE scope_guard::scope_guard(function &&fn):
	m_fn(std::move(fn)),
	m_dismissed(false)
{
}

SGE_INLINE scope_guard::~scope_guard(void)
{
	if (m_fn && !m_dismissed)
		m_fn();
}

SGE_INLINE void scope_guard::dismiss(void)
{
	m_dismissed = true;
}

SGE_END

#endif // SGE_SCOPEGUARD_HPP
