
target_compile_definitions(sge
	PRIVATE
		_LARGEFILE64_SOURCE
	)

target_sources(sge
	PRIVATE
		miniz.c
		miniz_tdef.c
		miniz_tinfl.c
		miniz_zip.c
	)

