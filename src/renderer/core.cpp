//
//
#include "SGL_gl.h"
#include "SGL_game.h"
#include "SGL_renderer.h"

SGL_RENDERER_BEGIN

enum Uniform_v1f {
	UNIFORM_V1F_ELAPSED,
	UNIFORM_V1F_MAX
};

enum Uniform_m4f {
	UNIFORM_M4F_MODEL_MATRIX,
	UNIFORM_M4F_VIEW_MATRIX,
	UNIFORM_M4F_PROJECT_MATRIX,
	UNIFORM_M4F_NORMAL_MATRIX,
	UNIFORM_M4F_MAX
};

static GLint			Uniform_m4f_loc[UNIFORM_M4F_MAX];
static Eigen::Matrix4f	Uniform_m4f[UNIFORM_M4F_MAX];
static GLint			Uniform_v1f_loc[UNIFORM_V1F_MAX];
static float			Uniform_v1f[UNIFORM_V1F_MAX];
static Eigen::Vector4i	Viewport;
static GL::Program		Program;

static const char *VertexShaderSource =
	"#version 330 core\n"
	"uniform mat4 sge_ModelMatrix;\n"
	"uniform mat4 sge_ViewMatrix;\n"
	"uniform mat4 sge_ProjectMatrix;\n"
	"uniform mat4 sge_NormalMatrix;\n"
	"uniform float sge_Elapsed;\n"
	"layout(location = 0) in vec3 Position;\n"
	"layout(location = 1) in vec3 Normal;\n"
	"void main() {\n"
	"    gl_Position = sge_ProjectMatrix * sge_ViewMatrix * sge_ModelMatrix * vec4(Position, 1.0);\n"
	"}";

static const char *FragmentShaderSource =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}";

static bool InitProgram(void)
{
	if (!Program.Create())
		return false;

	if (!Program.AddShader(GL_VERTEX_SHADER, VertexShaderSource)) {
		Program.Destroy();
		return false;
	}

	if (!Program.AddShader(GL_FRAGMENT_SHADER, FragmentShaderSource)) {
		Program.Destroy();
		return false;
	}

	if (!Program.Link()) {
		Program.Destroy();
		return false;
	}

	/* v1f */

	for (int i = 0; i < UNIFORM_V1F_MAX; ++i)
		Uniform_v1f_loc[i] = -1;

	Uniform_v1f_loc[UNIFORM_V1F_ELAPSED] = Program.GetUniformLocation("sge_Elapsed");

	/* m4f */

	for (int i = 0; i < UNIFORM_M4F_MAX; ++i)
		Uniform_m4f_loc[i] = -1;

	Uniform_m4f_loc[UNIFORM_M4F_MODEL_MATRIX] = Program.GetUniformLocation("sge_ModelMatrix");
	SGL_LOGD("Uniform_m4f_loc[UNIFORM_M4F_MODEL_MATRIX] %d\n", Uniform_m4f_loc[UNIFORM_M4F_MODEL_MATRIX]);
	Uniform_m4f_loc[UNIFORM_M4F_VIEW_MATRIX] = Program.GetUniformLocation("sge_ViewMatrix");
	SGL_LOGD("Uniform_m4f_loc[UNIFORM_M4F_VIEW_MATRIX] %d\n", Uniform_m4f_loc[UNIFORM_M4F_VIEW_MATRIX]);
	Uniform_m4f_loc[UNIFORM_M4F_PROJECT_MATRIX] = Program.GetUniformLocation("sge_ProjectMatrix");
	SGL_LOGD("Uniform_m4f_loc[UNIFORM_M4F_PROJECT_MATRIX] %d\n", Uniform_m4f_loc[UNIFORM_M4F_PROJECT_MATRIX]);
	Uniform_m4f_loc[UNIFORM_M4F_NORMAL_MATRIX] = Program.GetUniformLocation("sge_NormalMatrix");
	SGL_LOGD("Uniform_m4f_loc[UNIFORM_M4F_NORMAL_MATRIX] %d\n", Uniform_m4f_loc[UNIFORM_M4F_NORMAL_MATRIX]);

	return true;
}

static void CommitUniforms(void)
{
	for (int i = 0; i < UNIFORM_V1F_MAX; ++i) {
		if (Uniform_v1f_loc[i] >= 0)
			Program.Uniform(Uniform_v1f_loc[i], Uniform_v1f[i]);
	}

	for (int i = 0; i < UNIFORM_M4F_MAX; ++i) {
		if (Uniform_m4f_loc[i] >= 0)
			Program.Uniform(Uniform_m4f_loc[i], 1, GL_FALSE, &Uniform_m4f[i]);
	}
}

static void DrawBegin(void)
{
	// cull?
}

static void DrawEnd(void)
{
	glUseProgram(Program.Id());

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	CommitUniforms();
}

static void HandleWindowEvent(const SDL_WindowEvent *event)
{
	SGL_ASSERT(event != NULL);

	if (event->windowID != GL::GetWindowID())
		return;

	switch (event->event) {
	case SDL_WINDOWEVENT_RESIZED:
		break;
	}
}

bool Init(void)
{
	SGL_LOGI("Renderer initializing...\n");

	if (!InitProgram())
		return false;

	return true;
}

void Shutdown(void)
{
	Program.Destroy();

	SGL_LOGI("Renderer shutdown.\n");
}

void Draw(float elapsed)
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

void AddObject(Object *obj)
{
	SDL_assert(obj != NULL);
}

void RemoveObject(Object *obj)
{
	SDL_assert(obj != NULL);
}

SGL_RENDERER_END
