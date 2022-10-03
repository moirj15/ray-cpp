add_library(d3dViewerLib STATIC
        SceneViewer.cpp)

target_include_directories(d3dViewerLib PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/SDL2/include
        ${CMAKE_SOURCE_DIR}/libs/glm/

        )

add_executable(d3d_viewer
        main.cpp )

target_link_libraries(d3d_viewer PRIVATE
        d3dViewerLib
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
        d3d11.lib
        d3dcompiler.lib
        )
