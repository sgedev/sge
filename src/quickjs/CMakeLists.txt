
sge_compile_definitions(
	CONFIG_VERSION=\"2019-10-27\"
	)

if(UNIX)
	sge_compile_definitions(_GNU_SOURCE)
	sge_link_libraries(dl)
endif()

sge_sources(
	cutils.c
	cutils.h
	libbf.c
	libbf.h
	libregexp.c
	libregexp.h
	libregexp-opcode.h
	libunicode.c
	libunicode.h
	libunicode-table.h
	list.h
	quickjs.c
	quickjs-atom.h
	quickjs-libc.c
	quickjs-libc.h
	quickjs-opcode.h
	)

