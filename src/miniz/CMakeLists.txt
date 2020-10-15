
if(UNIX)
    target_compile_definitions(sge
        PRIVATE
            _GNU_SOURCE
            _LARGEFILE_SOURCE
            _LARGEFILE64_SOURCE
            _FILE_OFFSET_BITS=64
        )
endif()

target_sources(sge
    PRIVATE
        miniz.c
        miniz_tdef.c
        miniz_tinfl.c
        miniz_zip.c
    )
