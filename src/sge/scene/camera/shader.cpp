//
//
#include <sge/scene/camera/common.hpp>

SGE_SCENE_CAMERA_BEGIN

const char *g_vertex_shader_source =
	"#version 130\n"
	"uniform mat4 sge_model_matrix;\n"
	"uniform mat4 sge_view_matrix;\n"
	"uniform mat4 sge_projection_matrix;\n"
	"uniform mat4 sge_model_view_matrix;\n"
	"uniform mat4 sge_mvp_matrix;\n"
	"uniform mat4 sge_normal_matrix;\n"
	"uniform float sge_elapsed;\n"
	"void main() {\n"
	"	gl_Position = vec4(0.0, 0.0, 0.0, 1.0);\n"
	"}\n"
	;

const char *g_fragment_shader_source =
	"#version 130\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
	"	FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
	"}\n"
	;

SGE_SCENE_CAMERA_END

