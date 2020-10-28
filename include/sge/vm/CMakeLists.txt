
target_sources(sge
    PRIVATE
        common.hpp
        core.hpp
        fs.hpp
        kernel.hpp
        root_fs.hpp
        task.h
        zip_fs.hpp
    )

target_compile_definitions(sge PRIVATE LUA_USER_H=\"${CMAKE_CURRENT_SOURCE_DIR}/task.h\")
