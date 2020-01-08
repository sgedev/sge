//
//
#include <sge/camera.hpp>

SGE_BEGIN

Camera::Camera(Node *parent)
	: Node(parent)
{
}

Camera::~Camera(void)
{
}

bool Camera::canSee(const Node *target) const
{
	Q_ASSERT(target != Q_NULLPTR);

	return true;
}

void Camera::drawNode(View &view) const
{
	// draw a camera icon into view.
}

SGE_END

