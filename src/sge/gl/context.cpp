//
//
#include <sge/gl/context.hpp>

SGE_GL_BEGIN

context *context::c_current = NULL;

const char *context::c_vs_sources[context::PROGRAM_MAX] = {
	// PROGRAM_DEFAULT
	"void main() {\n"
	"	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;\n"
	"}\n",
};

const char *context::c_fs_sources[context::PROGRAM_MAX] = {
	// PROGRAM_DEFAULT
	"void main() {\n"
	"	gl_FragColor = vec4(0.4,0.4,0.8,1.0);\n"
	"}\n",
};

context::context(void)
	: m_window(NULL)
	, m_gl_context(NULL)
	, m_drawing(false)
{
}

context::~context(void)
{
	if (m_gl_context)
		shutdown();
}

bool context::init(SDL_Window *w)
{
	SGE_ASSERT(m_window == NULL);
	SGE_ASSERT(w != NULL);

	SGE_LOGD("Initializing OpenGL...\n");

	m_gl_context = SDL_GL_CreateContext(w);
	if (m_gl_context == NULL) {
		SGE_LOGE("Failed to create OpenGL context.\n");
		return false;
	}

	SDL_GL_MakeCurrent(w, m_gl_context);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		SGE_LOGE("Failed to initialize GLEW.\n");
		SDL_GL_DeleteContext(m_gl_context);
		m_gl_context = NULL;
		return false;
	}

#ifdef SGE_DEBUG
	//if (GL_KHR_debug) {
	//	SGE_LOGD("Enable OpenGL debug output.\n");
	//	glDebugMessageCallback(debug_output, NULL);
	//}
#endif

	m_window = w;

	SGE_LOGI("OpenGL: %s\n", glGetString(GL_VERSION));

	init_programs();

	return true;
}

void context::shutdown(void)
{
	SGE_ASSERT(m_window != NULL);
	SGE_ASSERT(m_gl_context != NULL);
	SGE_ASSERT(!m_drawing);

	if (m_gl_context == SDL_GL_GetCurrentContext())
		SDL_GL_MakeCurrent(m_window, NULL);

	SDL_GL_DeleteContext(m_gl_context);
	m_gl_context = NULL;

	m_window = NULL;
}

context *context::get_current(void)
{
	return c_current;
}

bool context::init_programs(void)
{
	for (int i = 0; i < PROGRAM_MAX; ++i) {
		program &prog = m_program_table[i];
		prog.create();
		prog.add_shader(GL_VERTEX_SHADER, c_vs_sources[i]);
		prog.add_shader(GL_FRAGMENT_SHADER, c_fs_sources[i]);
	}

	return true;
}

SGE_GL_END

