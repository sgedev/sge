#
#
#

target_sources(sge
	PUBLIC
		sge.h

	PRIVATE
		sge_config.h.in
	)

configure_file(
	sge_config.h.in
	sge_config.h
	)

