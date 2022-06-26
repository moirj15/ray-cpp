project(rayTracer)

cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
        rayTracer
        src/main.cpp
		src/bmpIO.hpp
        src/bmpIO.cpp
		src/tracer.hpp
		src/tracer.cpp
		src/object.hpp
        src/object.cpp
		src/scene.hpp
        src/scene.cpp
		src/utils.hpp
        src/utils.cpp
        src/shader.cpp
		src/shader.hpp
        src/intersectData.cpp
        src/intersectData.h
		src/ray.hpp
		src/material.hpp
		src/material.cpp
        src/SceneViewer.cpp src/camera.hpp)

target_include_directories(rayTracer
        PRIVATE
        libs/glm
        libs/sdl2/include
)

target_link_libraries(
        rayTracer
        Setupapi.lib
        ${CMAKE_SOURCE_DIR}/libs/SDL2/SDL2main.lib
        ${CMAKE_SOURCE_DIR}/libs/SDL2/SDL2-static.lib
        msvcrtd.lib
        winmm.lib
        imm32.lib
        version.lib
		d3d11.lib
)
