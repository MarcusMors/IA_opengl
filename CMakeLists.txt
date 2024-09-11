cmake_minimum_required(VERSION 3.10)

project(MatrixDisplay)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLU REQUIRED)

add_executable(matrix_display main.cpp)

target_link_libraries(matrix_display OpenGL::GL GLUT::GLUT GLU)
