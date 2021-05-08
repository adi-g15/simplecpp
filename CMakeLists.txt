cmake_minimum_required(VERSION 3.14)

project(simplecpp)

include_directories(include "C:\\Program Files (x86)\\GLFW\\include")
link_directories("C:\\Program Files (x86)\\GLFW\\bin" "C:\\Program Files (x86)\\GLFW\\lib")
link_libraries("glfw3dll.lib")
add_library( glad "src/glad.c" )
add_executable(opengl "examples/opengl.cpp" "src/globals.cpp")
target_link_libraries(opengl glad)
