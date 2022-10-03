add_library(importerLib
        STATIC
        scene_importer.cpp
        file_io.cpp
        )

target_include_directories(importerLib
        PRIVATE
        ${CMAKE_SOURCE_DIR}/libs/glm
        )
target_link_directories(importerLib
        PRIVATE
        )
