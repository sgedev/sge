//
//
#ifndef SGE_CAMERA_H
#define SGE_CAMERA_H

#include <sge/common.hpp>
#include <sge/gl.hpp>

SGE_BEGIN

class camera {
public:
	camera(void);
	virtual ~camera(void);

public:
	bool init(void);
	void shutdown(void);
	void update(float elapsed);
	void draw(void);

private:
	
};

SGE_END

#endif // SGE_CAMERA_H

