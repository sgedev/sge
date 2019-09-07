//
//
#ifndef SGE_COMMON_HPP
#define SGE_COMMON_HPP

#include <cstdint>

#include <glm.hpp>

#include <sge/common.h>

#define SGE_BEGIN namespace sge {
#define SGE_END }

SGE_BEGIN

typedef std::int8_t int8_t;
typedef std::uint8_t uint8_t;
typedef std::int16_t int16_t;
typedef std::uint16_t uint16_t;
typedef std::int32_t int32_t;
typedef std::uint32_t uint32_t;
typedef std::int64_t int64_t;
typedef std::uint64_t uint64_t;

class noncopyable {
public:
	noncopyable(void) { }
	~noncopyable(void) { }

private:
	noncopyable(const noncopyable &);
	noncopyable &operator=(const noncopyable &);
};

SGE_END

#endif // SGE_COMMON_HPP

