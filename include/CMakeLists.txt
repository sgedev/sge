
target_sources(sge
    PRIVATE
        argh.h
        debug_draw.hpp
        fontstash.h
        lauxlib.h
        lua.h
        lua.hpp
        luaconf.h
        lualib.h
        miniz.h
        miniz_common.h
        miniz_tdef.h
        miniz_tinfl.h
        miniz_zip.h
        nanovg.h
        nanovg_gl.h
        nanovg_gl_utils.h
        oui.h
        stb_image.h
        stb_truetype.h
        uv.h
    )

add_subdirectory(GL GL)
add_subdirectory(glm glm)
add_subdirectory(KHR KHR)
add_subdirectory(rttr rttr)
add_subdirectory(sge sge)
add_subdirectory(uv uv)
