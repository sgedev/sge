
if(WIN32)
	sge_sources(pfs_windows.c)
else()
	sge_sources(pfs_linux.c)
endif()

