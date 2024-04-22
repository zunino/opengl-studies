cmake_minimum_required(VERSION 3.10)

project(orthographic VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(inc)
add_executable(ortho src/main.cpp src/shader_prog.cpp src/geometry.cpp)

add_library(glad STATIC 3rd/glad/glad.c)

target_link_libraries(ortho glad GL glfw ${CMAKE_DL_LIBS})
