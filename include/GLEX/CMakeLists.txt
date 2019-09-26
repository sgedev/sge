
target_sources(sge
	PUBLIC
		common.hpp
		context.hpp
		light.hpp
		material.hpp
		mesh.hpp
	PRIVATE
		config.hpp.in
	)

if(SGE_DEBUG)
	set(GLEX_DEBUG ON)
endif()

configure_file(
	config.hpp.in
	config.hpp
)

