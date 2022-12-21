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
        ../Camera.cpp
 "SoftwareRenderer.cpp")

target_include_directories(softwareRendererLib PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/glm/
)

add_executable(software_renderer
		main.cpp
        window.h
        window.cpp
 "SoftwareRenderer.cpp")

target_include_directories(software_renderer PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/SDL2/include
        ${CMAKE_SOURCE_DIR}/libs/glm/
        geometryLib
        importerLib
)

target_link_libraries(software_renderer PRIVATE
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
