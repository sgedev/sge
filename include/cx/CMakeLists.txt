
target_sources(sge
	PUBLIC
		common.h
		common.hpp
		config.h.in
		list.h
		noncopyable.hpp
		signal.hpp
	)

configure_file(
	config.h.in
	config.h
	)

