cmake_minimum_required(VERSION 3.28)
project(Projeto1 C)

set(CMAKE_C_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(Projeto1)

include_directories(/opt/homebrew/include)

add_executable(Projeto1
        Projeto1/DBForms.c
        Projeto1/DBForms.h
        Projeto1/form.c
        Projeto1/form.h
        Projeto1/main.c
)


target_link_libraries(Projeto1 OpenGL::GL GLUT::GLUT)
