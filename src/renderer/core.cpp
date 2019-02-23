//
//
#include <sge/gl.h>
#include <sge/renderer.h>

SGE_RENDERER_BEGIN

enum uniform_v1f {
	UNIFORM_V1F_ELAPSED,
	UNIFORM_V1F_MAX
};

enum uniform_m4f {
	UNIFORM_M4F_MODEL_MATRIX,
	UNIFORM_M4F_VIEW_MATRIX,
	UNIFORM_M4F_PROJECT_MATRIX,
	UNIFORM_M4F_NORMAL_MATRIX,
	UNIFORM_M4F_MAX
};

static GLint			uniform_m4f_loc[UNIFORM_M4F_MAX];
static Eigen::Matrix4f	uniform_m4f[UNIFORM_M4F_MAX];
static GLint			uniform_v1f_loc[UNIFORM_V1F_MAX];
static float			uniform_v1f[UNIFORM_V1F_MAX];
static Eigen::Vector4i	viewport;
static GL::Program		program;

static const char *vertex_shader_source =
	"#version 330 core\n"
	"uniform mat4 sge_model_matrix;\n"
	"uniform mat4 sge_view_matrix;\n"
	"uniform mat4 sge_project_matrix ;\n"
	"uniform mat4 sge_normal_matrix;\n"
	"uniform float sge_elapsed;\n"
	"layout(location = 0) in vec3 position;\n"
	"layout(location = 1) in vec3 normal;\n"
	"void main() {\n"
	"    gl_Position = sge_project_matrix  * sge_view_matrix * sge_model_matrix * vec4(position, 1.0);\n"
	"}";

static const char *fragment_shader_source =
	"#version 330 core\n"
	"out vec4 frag_color;\n"
	"void main() {\n"
	"	frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}";

static bool InitProgram(void)
{
	if (!program.Create())
		return false;

	if (!program.AddShader(GL_VERTEX_SHADER, vertex_shader_source)) {
		program.Destroy();
		return false;
	}

	if (!program.AddShader(GL_FRAGMENT_SHADER, fragment_shader_source)) {
		program.Destroy();
		return false;
	}

	if (!program.Link()) {
		program.Destroy();
		return false;
	}

	/* v1f */

	for (int i = 0; i < UNIFORM_V1F_MAX; ++i)
		uniform_v1f_loc[i] = -1;

	uniform_v1f_loc[UNIFORM_V1F_ELAPSED] = program.GetUniformLocation("sge_elapsed");

	/* m4f */

	for (int i = 0; i < UNIFORM_M4F_MAX; ++i)
		uniform_m4f_loc[i] = -1;

	uniform_m4f_loc[UNIFORM_M4F_MODEL_MATRIX] = program.GetUniformLocation("sge_model_matrix");
	SGE_LOGD("UNIFORM_M4F_MODEL_MATRIX %d\n", uniform_m4f_loc[UNIFORM_M4F_MODEL_MATRIX]);
	uniform_m4f_loc[UNIFORM_M4F_VIEW_MATRIX] = program.GetUniformLocation("sge_view_matrix");
	SGE_LOGD("UNIFORM_M4F_VIEW_MATRIX %d\n", uniform_m4f_loc[UNIFORM_M4F_VIEW_MATRIX]);
	uniform_m4f_loc[UNIFORM_M4F_PROJECT_MATRIX] = program.GetUniformLocation("sge_project_matrix ");
	SGE_LOGD("UNIFORM_M4F_PROJECT_MATRIX %d\n", uniform_m4f_loc[UNIFORM_M4F_PROJECT_MATRIX]);
	uniform_m4f_loc[UNIFORM_M4F_NORMAL_MATRIX] = program.GetUniformLocation("sge_normal_matrix");
	SGE_LOGD("UNIFORM_M4F_NORMAL_MATRIX %d\n", uniform_m4f_loc[UNIFORM_M4F_NORMAL_MATRIX]);

	return true;
}

static void CommitUniforms(void)
{
	for (int i = 0; i < UNIFORM_V1F_MAX; ++i) {
		if (uniform_v1f_loc[i] >= 0)
			program.Uniform(uniform_v1f_loc[i], uniform_v1f[i]);
	}

	for (int i = 0; i < UNIFORM_M4F_MAX; ++i) {
		if (uniform_m4f_loc[i] >= 0)
			program.Uniform(uniform_m4f_loc[i], 1, GL_FALSE, &uniform_m4f[i]);
	}
}

static void DrawBegin(void)
{
	// cull?
}

static void DrawEnd(void)
{
}

static void HandleWindowEvent(const SDL_WindowEvent *event)
{
	SGE_ASSERT(event != NULL);

	if (event->windowID != GL::GetWindowID())
		return;

	switch (event->event) {
	case SDL_WINDOWEVENT_RESIZED:
		break;
	}
}

bool Init(void)
{
	SGE_LOGI("Initializing renderer...\n");

	if (!InitProgram())
		return false;

	return true;
}

void Shutdown(void)
{
	SGE_LOGI("Shuting down renderer...\n");

	program.Destroy();
}

void Update(void)
{

}

void Draw(void)
{
	DrawBegin();

	/* TODO add visible surfaces here. */

	DrawEnd();
}

void HandleEvent(const SDL_Event *event)
{
	switch (event->type) {
	case SDL_WINDOWEVENT:
		HandleWindowEvent(&event->window);
		break;
	}
}

SGE_RENDERER_END
