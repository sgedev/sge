//
//
#include <sge/gl.hpp>
#include <sge/renderer.hpp>

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
static gl::program		program;

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
	"    gl_Position = sge_project_matrix  * sge_view_matrix * sge_model_matrix * vec4(Position, 1.0);\n"
	"}";

static const char *fragment_shader_source =
	"#version 330 core\n"
	"out vec4 frag_color;\n"
	"void main() {\n"
	"	frag_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}";

static bool init_program(void)
{
	if (!program.create())
		return false;

	if (!program.add_shader(GL_VERTEX_SHADER, vertex_shader_source)) {
		program.destroy();
		return false;
	}

	if (!program.add_shader(GL_FRAGMENT_SHADER, fragment_shader_source)) {
		program.destroy();
		return false;
	}

	if (!program.link()) {
		program.destroy();
		return false;
	}

	/* v1f */

	for (int i = 0; i < UNIFORM_V1F_MAX; ++i)
		uniform_v1f_loc[i] = -1;

	uniform_v1f_loc[UNIFORM_V1F_ELAPSED] = program.get_uniform_location("sge_elapsed");

	/* m4f */

	for (int i = 0; i < UNIFORM_M4F_MAX; ++i)
		uniform_m4f_loc[i] = -1;

	uniform_m4f_loc[UNIFORM_M4F_MODEL_MATRIX] = program.get_uniform_location("sge_model_matrix");
	SGE_LOGD("Uniform_m4f_loc[UNIFORM_M4F_MODEL_MATRIX] %d\n", uniform_m4f_loc[UNIFORM_M4F_MODEL_MATRIX]);
	uniform_m4f_loc[UNIFORM_M4F_VIEW_MATRIX] = program.get_uniform_location("sge_view_matrix");
	SGE_LOGD("Uniform_m4f_loc[UNIFORM_M4F_VIEW_MATRIX] %d\n", uniform_m4f_loc[UNIFORM_M4F_VIEW_MATRIX]);
	uniform_m4f_loc[UNIFORM_M4F_PROJECT_MATRIX] = program.get_uniform_location("sge_project_matrix ");
	SGE_LOGD("Uniform_m4f_loc[UNIFORM_M4F_PROJECT_MATRIX] %d\n", uniform_m4f_loc[UNIFORM_M4F_PROJECT_MATRIX]);
	uniform_m4f_loc[UNIFORM_M4F_NORMAL_MATRIX] = program.get_uniform_location("sge_normal_matrix");
	SGE_LOGD("Uniform_m4f_loc[UNIFORM_M4F_NORMAL_MATRIX] %d\n", uniform_m4f_loc[UNIFORM_M4F_NORMAL_MATRIX]);

	return true;
}

static void commit_uniforms(void)
{
	for (int i = 0; i < UNIFORM_V1F_MAX; ++i) {
		if (uniform_v1f_loc[i] >= 0)
			program.uniform(uniform_v1f_loc[i], uniform_v1f[i]);
	}

	for (int i = 0; i < UNIFORM_M4F_MAX; ++i) {
		if (uniform_m4f_loc[i] >= 0)
			program.uniform(uniform_m4f_loc[i], 1, GL_FALSE, &uniform_m4f[i]);
	}
}

static void draw_begin(void)
{
	// cull?
}

static void draw_end(void)
{
}

static void handle_window_event(const SDL_WindowEvent *event)
{
	SGE_ASSERT(event != NULL);

	if (event->windowID != gl::get_window_id())
		return;

	switch (event->event) {
	case SDL_WINDOWEVENT_RESIZED:
		break;
	}
}

bool init(void)
{
	SGE_LOGI("Renderer initializing...\n");

	if (!init_program())
		return false;

	return true;
}

void shutdown(void)
{
	program.destroy();

	SGE_LOGI("Renderer shutdown.\n");
}

void draw(void)
{
	draw_begin();

	/* TODO add visible surfaces here. */

	draw_end();
}

void handle_event(const SDL_Event *event)
{
	switch (event->type) {
	case SDL_WINDOWEVENT:
		handle_window_event(&event->window);
		break;
	}
}

SGE_RENDERER_END
