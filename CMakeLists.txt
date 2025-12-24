cmake_minimum_required(VERSION 3.10.0)
project(LearningOpenGL VERSION 0.1.0)

find_package(OpenGL REQUIRED)

add_executable(LearningOpenGL
    src/config.h
    src/main.cpp
    src/app.h
    src/funnel.h
    src/generator.h
    src/geometry.h
    src/pathfind.h
    src/triangulate.h
    src/glad.c
)

set(DEPENDENCIES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)

target_include_directories(LearningOpenGL PRIVATE 
    ${DEPENDENCIES_PATH}/GLFW/include
    ${DEPENDENCIES_PATH}/GLAD/include
    ${DEPENDENCIES_PATH}/GLM/include
)
target_link_directories(LearningOpenGL PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW/lib-vc2022
)

target_link_libraries(LearningOpenGL PRIVATE 
    glfw3
    OpenGL::GL
)


