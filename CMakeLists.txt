cmake_minimum_required(VERSION 3.22)
project(Snake_2 C)

set(CMAKE_C_STANDARD 99)

add_executable(Snake_2 main.c input.c input.h main.h)
target_link_libraries(Snake_2 freeglut opengl32 glu32)