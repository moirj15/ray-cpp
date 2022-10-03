
add_library(
        rayTracerLib
        STATIC
        tracer.cpp
)

target_include_directories(rayTracerLib
        PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/glm
        geometryLib
)

target_link_libraries(
        rayTracerLib
        geometryLib
        Setupapi.lib
        ${CMAKE_SOURCE_DIR}/libs/SDL2/SDL2-static.lib
        msvcrtd.lib
        winmm.lib
        imm32.lib
        version.lib
)

add_executable(cmdLineRayTracer
        main.cpp
        bmpIO.cpp
)

target_link_libraries(cmdLineRayTracer
        rayTracerLib
        importerLib
        )

target_include_directories(cmdLineRayTracer
        PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/glm
        ${CMAKE_SOURCE_DIR}/libs/sdl2/include
)
