//
//
#include <SGE/OpenGLRenderer.hpp>

SGE_BEGIN

OpenGLRenderer::OpenGLRenderer(QObject *parent):
	Renderer(parent)
{
}

OpenGLRenderer::~OpenGLRenderer(void)
{
}

bool OpenGLRenderer::init(OpenGLFunctions *funcs)
{
	Q_ASSERT(funcs != NULL);

	Q_ASSERT(m_funcs == NULL);
	m_funcs = funcs;

	return true;
}

void OpenGLRenderer::addObject(ObjectPtr obj)
{

}

void OpenGLRenderer::removeObject(ObjectPtr obj)
{

}

SGE_END
