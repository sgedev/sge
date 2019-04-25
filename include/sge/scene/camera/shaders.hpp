//
//
#ifndef SGE_SCENE_CAMERA_SHADERS_HPP
#define SGE_SCENE_CAMERA_SHADERS_HPP

#include <sge/scene/camera/common.hpp>

SGE_SCENE_CAMERA_BEGIN

namespace shaders {
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
}

SGE_SCENE_CAMERA_END

#endif // SGE_SCENE_CAMERA_SHADERS_HPP

