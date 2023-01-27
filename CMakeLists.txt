cmake_minimum_required(VERSION 3.0.0)
project(render VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories("C:/cppLibraries/glfw-3.3.8.bin.WIN64/include/GLFW")
include_directories("C:/cppLibraries/glad/include")
include_directories("C:/cppLibraries/glad/src")
include_directories("C:/cppLibraries/glm/glm")

add_executable(render 
main.cpp
C:/cppLibraries/glad/src/glad.c
shader.h
cameraUtils.h
stb_image.h)


target_link_directories(render PUBLIC "C:/cppLibraries/glfw-3.3.8.bin.WIN64/lib-mingw-w64")
target_link_libraries(render glfw3)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
