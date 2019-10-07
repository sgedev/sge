
target_sources(sge
	PUBLIC
		common.h
		common.hpp
		list.h
		noncopyable.hpp
		signal.hpp
	PRIVATE
		config.h.in
	)

configure_file(
	config.h.in
	config.h
	)

