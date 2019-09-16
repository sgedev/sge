
sge_sources(
	gl3w.h
	glcorearb.h
	glex.h
	glexconfig.h.in
	)

if(SGE_DEBUG)
	set(GLEX_DEBUG ON)
endif()

configure_file(
	glexconfig.h.in
	glexconfig.h
)
