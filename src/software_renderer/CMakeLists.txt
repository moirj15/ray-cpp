add_library(softwareRendererLib STATIC	
        canvas.cpp
        polygon.cpp
        rasterizer.cpp
        utils.cpp
        clipper.cpp
        vbo.cpp
        obj.cpp
        transform.cpp
        lighting.cpp
        triangulation.cpp
        triangleRasterizer.cpp
)

target_include_directories(softwareRendererLib PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/glm/
)

add_executable(softwareRenderer
		main.cpp
        window.h
        window.cpp
)

target_include_directories(softwareRenderer PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/SDL2/include
        ${CMAKE_SOURCE_DIR}/libs/glm/
        geometryLib
        importerLib
)

target_link_libraries(softwareRenderer PRIVATE
        softwareRendererLib
        importerLib
        geometryLib
        ${CMAKE_SOURCE_DIR}/libs/sdl2/SDL2-static.lib
        ${CMAKE_SOURCE_DIR}/libs/sdl2/SDL2main.lib
        gdi32.lib
        user32.lib
        kernel32.lib
        msvcrtd.lib
        winmm.lib
        imm32.lib
        version.lib
        libcmt.lib
        dxgi.lib
        dxguid.lib
        winmm.lib
        gdi32
        Setupapi.lib
        )
