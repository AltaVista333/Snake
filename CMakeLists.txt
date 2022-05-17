cmake_minimum_required(VERSION 3.22)
project(Snake_2 C)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLUT REQUIRED)
set(CMAKE_C_STANDARD 99)

add_executable(Snake_2 view/view.c controller/input_controller.c
        controller/input_controller.h view/view.h model/structs.h
        logic/game.c logic/game.h util/logger.h util/logger.c game_settings.h
        menu/menumain.c menu/parser.c menu/parser.h menu/menu.c menu/menu.h menu/util.c menu/util.h
        main.c menu/menumain.h)
target_link_libraries(Snake_2 freeglut opengl32 glu32)