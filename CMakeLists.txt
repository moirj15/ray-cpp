project(rayTracer)

add_executable(
	rayTracer
	main.cpp
	bmpIO.h
	bmpIO.cpp
	camera.h
	camera.cpp
	object.h
	object.cpp
	world.h
	world.cpp
	utils.h
	utils.cpp
	illumination.cpp
	illumination.h
	intersectData.cpp
	intersectData.h
	ray.h
	ray.cpp
)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -pedantic")
set(CMAKE_EXE_LINKER_FLAGES "-lSDL2 -lm")
target_link_libraries(
	rayTracer
	SDL2
    m
)
