project(rayTracer)

cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
	rayTracer
	"src/main.cpp"
	"src/bmpIO.h"
	"src/bmpIO.cpp"
	"src/camera.h"
	"src/camera.cpp"
	"src/object.h"
	"src/object.cpp"
	src/scene.h
	src/scene.cpp
	"src/utils.h"
	"src/utils.cpp"
	"src/illumination.cpp"
	"src/illumination.h"
	"src/intersectData.cpp"
	"src/intersectData.h"
	"src/ray.h"
        src/material.h src/material.cpp)

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
)
