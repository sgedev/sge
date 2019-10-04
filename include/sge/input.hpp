//
//
#ifndef SGE_INPUT_HPP
#define SGE_INPUT_HPP

#include <map>
#include <functional>

#include <sge/common.hpp>
#include <sge/keycode.hpp>

SGE_BEGIN

class Input {
public:
	typedef std::function<void(void)> KeyDownHandler;

public:
	Input(void);
	virtual ~Input(void);


};

SGE_END

#endif // SGE_INPUT_HPP
