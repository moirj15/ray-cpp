
add_library(
        rayTracerLib
		bmpIO.hpp
        bmpIO.cpp
		tracer.hpp
		tracer.cpp
		object.hpp
        object.cpp
		scene.hpp
        scene.cpp
		utils.hpp
        utils.cpp
        shader.cpp
		shader.hpp
        intersectData.cpp
        intersectData.h
		ray.hpp
		material.hpp
		material.cpp
        SceneViewer.cpp
		camera.hpp
		frame.hpp
)

target_include_directories(rayTracerLib
        PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/glm
        ${CMAKE_SOURCE_DIR}/libs/sdl2/include
)

target_link_libraries(
        rayTracerLib
        Setupapi.lib
        ${CMAKE_SOURCE_DIR}/libs/SDL2/SDL2-static.lib
        msvcrtd.lib
        winmm.lib
        imm32.lib
        version.lib
)

add_executable(cmdLineRayTracer
		main.cpp
)

target_link_libraries(cmdLineRayTracer
		rayTracerLib)

target_include_directories(cmdLineRayTracer
		PRIVATE
		${CMAKE_SOURCE_DIR}/libs/glm
		)
