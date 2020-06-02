
add_subdirectory(SGE SGE)

target_sources(sge
	PUBLIC
		SGE.hpp
	PRIVATE
		fontstash.h
		miniz.h
		miniz_common.h
		miniz_export.h
		miniz_tinfl.h
		miniz_zip.h
		nanovg.h
		nanovg_gl.h
		nanovg_gl_utils.h
		stb_image.h
		stb_truetype.h
	)
