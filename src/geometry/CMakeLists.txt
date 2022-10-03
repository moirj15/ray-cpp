add_library(geometryLib
        STATIC
        material.cpp
        object.cpp
        scene.cpp
        shader.cpp
)

target_include_directories(geometryLib
        PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/glm
)
target_link_directories(geometryLib
        PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/glm
)
