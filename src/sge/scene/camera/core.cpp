//
//
#include <sge/gl.hpp>
#include <sge/scene/camera.hpp>

SGE_SCENE_CAMERA_BEGIN

enum uniform_float {
	UNIFORM_ELAPSED = 0,
	UNIFORM_FLOAT_MAX
};

enum uniform_mat4 {
	UNIFORM_MODEL_MATRIX = 0,
	UNIFORM_VIEW_MATRIX,
	UNIFORM_PROJECTION_MATRIX,
	UNIFORM_MODEL_VIEW_MATRIX,
	UNIFORM_MVP_MATRIX,
	UNIFORM_NORMAL_MATRIX,
	UNIFORM_MAT4_MAX
};

extern const char *g_vertex_shader_source;
extern const char *g_fragment_shader_source;

static bool s_drawing;
static gl::program s_program;

// float uniforms
static int   s_uniform_float_loc[UNIFORM_FLOAT_MAX];
static float s_uniform_float[UNIFORM_FLOAT_MAX];

// mat4 uniforms
static int       s_uniform_mat4_loc[UNIFORM_MAT4_MAX];
static glm::mat4 s_uniform_mat4[UNIFORM_MAT4_MAX];

static bool init_program(void)
{
	if (!s_program.create())
		return false;

	SGE_LOGD("Building vertex shader...\n");
	if (!s_program.add_shader(GL_VERTEX_SHADER, g_vertex_shader_source)) {
		SGE_LOGE("Failed to build vertex shader: %s\n", s_program.info_log());
		s_program.destroy();
		return false;
	}

	SGE_LOGD("Building fragment shader...\n");
	if (!s_program.add_shader(GL_FRAGMENT_SHADER, g_fragment_shader_source)) {
		SGE_LOGE("Failed to build fragment shader: %s\n", s_program.info_log());
		s_program.destroy();
		return false;
	}

	SGE_LOGD("Linking program...\n");
	if (!s_program.link()) {
		SGE_LOGE("Failed to link program: %s\n", s_program.info_log());
		s_program.destroy();
		return false;
	}

	// float
	s_uniform_float_loc[UNIFORM_ELAPSED] = s_program.uniform_location("sge_elapsed");

	// mat4
	s_uniform_mat4_loc[UNIFORM_MODEL_MATRIX] = s_program.uniform_location("sge_model_matrix");
	s_uniform_mat4_loc[UNIFORM_VIEW_MATRIX] = s_program.uniform_location("sge_view_matrix");
	s_uniform_mat4_loc[UNIFORM_PROJECTION_MATRIX] = s_program.uniform_location("sge_projection_matrix");
	s_uniform_mat4_loc[UNIFORM_MODEL_VIEW_MATRIX] = s_program.uniform_location("sge_model_view_matrix");
	s_uniform_mat4_loc[UNIFORM_MVP_MATRIX] = s_program.uniform_location("sge_mvp_matrix");
	s_uniform_mat4_loc[UNIFORM_NORMAL_MATRIX] = s_program.uniform_location("sge_normal_matrix");

	return true;
}

static void commit_uniforms(void)
{
	int i;

	// float
	for (i = 0; i < UNIFORM_FLOAT_MAX; ++i) {
		if (s_uniform_float_loc[i] >= 0)
			s_program.uniform(s_uniform_float_loc[i], s_uniform_float[i]);
	}

	// mat4
	for (i = 0; i < UNIFORM_MAT4_MAX; ++i) {
		if (s_uniform_mat4_loc[i] >= 0)
			s_program.uniform(s_uniform_mat4_loc[i], 1, false, &s_uniform_mat4[i]);
	}
}

static void init_test_cube(void)
{
}

bool init(void)
{
	if (!init_program())
		return false;

	s_drawing = false;

	s_uniform_mat4[UNIFORM_VIEW_MATRIX] = glm::mat4(1.0f);
	s_uniform_mat4[UNIFORM_PROJECTION_MATRIX] = glm::mat4(0.0f);

	return true;
}

void shutdown(void)
{
	SGE_ASSERT(!s_drawing);

	s_program.destroy();
}

void ortho(float left, float right, float bottom, float top, float znear, float zfar)
{
	s_uniform_mat4[UNIFORM_PROJECTION_MATRIX] = glm::orthoLH(left, right, bottom, top, znear, zfar);
}

void frustum(float left, float right, float bottom, float top, float znear, float zfar)
{
	s_uniform_mat4[UNIFORM_PROJECTION_MATRIX] = glm::frustum(left, right, bottom, top, znear, zfar);
}

void look_at(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up)
{
	s_uniform_mat4[UNIFORM_PROJECTION_MATRIX] = glm::lookAt(eye, target, up);
}

void look_to(const glm::vec3 &eye, const glm::vec3 &direction, const glm::vec3 &up)
{
	s_uniform_mat4[UNIFORM_PROJECTION_MATRIX] = glm::lookAt(eye, eye + direction, up);
}

void reset(void)
{
	SGE_ASSERT(!s_drawing);

	s_drawing = true;
}

void render(void)
{
	SGE_ASSERT(s_drawing);

	s_drawing = false;

	glUseProgram(s_program.id());

	commit_uniforms();
}

void draw_test_cube(const glm::mat4 &transform)
{
	SGE_ASSERT(s_drawing);
}

SGE_SCENE_CAMERA_END

