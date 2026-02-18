cmake_minimum_required(VERSION 4.1)
project(Mandelbrot C)

set(CMAKE_C_STANDARD 11)

add_executable(Mandelbrot src/main.c)
target_link_libraries(Mandelbrot PRIVATE raylib m)

file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/themes DESTINATION ${CMAKE_BINARY_DIR})