//
//
#ifndef SGE_SCENE_LOADER_HPP
#define SGE_SCENE_LOADER_HPP

#include <sge/noncopyable.hpp>
#include <sge/db.hpp>
#include <sge/scene/common.hpp>

SGE_SCENE_BEGIN

class loader: public noncopyable {
public:
	loader(void);
	virtual ~loader(void);

public:
	bool start(db::node node);
	bool is_started(void);
	void cancel(void);
	int percentage(void);

private:
};

SGE_SCENE_END

#endif // SGE_SCENE_LOADER_HPP
