
sge_sources(
	common.h
	common.hpp
	list.h
	noncopyable.hpp
	signal.hpp
	config.h.in
	)

configure_file(
	config.h.in
	config.h
	)

