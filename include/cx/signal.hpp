//
// From:
//  https://simmesimme.github.io/tutorials/2015/09/20/signal-slot
//
#ifndef SGE_SIGNAL_HPP
#define SGE_SIGNAL_HPP

#include <functional>
#include <map>

#include <cx/noncopyable.hpp>

CX_BEGIN

template <typename... Args>
class signal: public noncopyable {
public:
	signal(void)
		: m_current_id(0)
	{
	}

public:
	template <typename T>
	int connect_member(T *inst, void (T::*func)(Args...))
	{
		return connect([=](Args... args) { (inst->*func)(args...); });
	}

	template <typename T>
	int connect_member(T *inst, void (T::*func)(Args...) const)
	{
		return connect([=](Args... args) { (inst->*func)(args...); });
	}

	int connect(const std::function<void(Args...)> &slot) const
	{
		m_slots.insert(std::make_pair(++m_current_id, slot));
		return m_current_id;
	}

	void disconnect(int id) const
	{
		m_slots.erase(id);
	}

	void disconnect_all() const
	{
		m_slots.clear();
	}

	void emit(Args... p)
	{
		for (auto it: m_slots)
			it.second(p...);
	}

private:
	typedef std::map <
		int, std::function<void(Args...)>
	> slots_type;

private:
	mutable slots_type m_slots;
	mutable int m_current_id;
};

CX_END

#endif // CX_SIGNAL_HPP

