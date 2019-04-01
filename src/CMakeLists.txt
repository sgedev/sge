#
#
#

add_subdirectory(glew glew)
add_subdirectory(hmm hmm)
add_subdirectory(miniz miniz)
add_subdirectory(sxmlc sxmlc)

target_sources(sge
	PRIVATE
		sge_camera.c
		sge_camera.h
		sge_camera_opengl.c
		sge_camera_software.c
		sge_common.c
		sge_common.h
		sge_fs.c
		sge_fs.h
		sge_hmm.c
		sge_hmm.h
		sge_list.c
		sge_list.h
		sge_scene.c
		sge_scene.h
	)

