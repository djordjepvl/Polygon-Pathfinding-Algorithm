cmake_minimum_required(VERSION 3.10.0)
project(LearningOpenGL VERSION 0.1.0)

find_package(OpenGL REQUIRED)

add_executable(LearningOpenGL
    src/glad.c
    src/config.h
    src/main.cpp

    src/app/app.h
    src/app/app.cpp
    src/app/geometry.h
    src/app/geometry.cpp
    src/app/util.h
    src/app/util.cpp

    src/app/algorithms/funnel.h
    src/app/algorithms/funnel.cpp
    src/app/algorithms/generator.h
    src/app/algorithms/generator.cpp
    src/app/algorithms/pathfind.h
    src/app/algorithms/pathfind.cpp
    src/app/algorithms/triangulate.h
    src/app/algorithms/triangulate.cpp
)

set(DEPENDENCIES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)

target_include_directories(LearningOpenGL PRIVATE 
    ${DEPENDENCIES_PATH}/GLFW/include
    ${DEPENDENCIES_PATH}/GLAD/include
    ${DEPENDENCIES_PATH}/GLM/include

    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app/algorithms
)
target_link_directories(LearningOpenGL PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/lib-vc2022
)

target_link_libraries(LearningOpenGL PRIVATE 
    glfw3
    OpenGL::GL
)


