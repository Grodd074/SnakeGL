cmake_minimum_required(VERSION 2.8>)
project(SnakeGL C)

set(CMAKE_C_STANDARD 99)

#
add_compile_options(-Wall -Wextra -pedantic)

add_executable(${PROJECT_NAME} main.c dependencies/linmath.h dependencies/glad/glad.c dependencies/glad/glad.h include/shader.h shader.c include/Camera.h Camera.c include/Player.h Player.c include/Tabuleiro.h Tabuleiro.c include/Estado.h Estado.c include/Mesh.h Mesh.c include/Model.h Model.c include/Texture.h Texture.c)

################# Detect and add OpenGL ########################
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
if (OPENGL_FOUND)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
    # or: target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_gl_LIBRARY})
    # target_link_libraries(${EXECUTABLE_NAME} m)  # if you use maths.h
endif()

################################################################
add_library(GLAD "dependencies/glad/glad.c")
target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/dependencies/GLFW/lib/libglfw3.a ${PROJECT_SOURCE_DIR}/dependencies/assimp/lib/libassimpd.dll.a)