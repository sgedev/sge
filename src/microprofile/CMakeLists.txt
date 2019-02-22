#
#
#

set(SGL_PROFILE OFF CACHE BOOL "Enable profile (microprofile).")
if(SGL_PROFILE)
	target_sources(sge
		PRIVATE
			microprofile.cpp
			microprofile.h
			microprofile_html.h
			microprofile.config.h
		)
	target_compile_definitions(sge
		PRIVATE
			MICROPROFILE_USE_CONFIG
		)
endif()
