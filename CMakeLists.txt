cmake_minimum_required(VERSION 3.10)

project(Learn-OpenGL)

find_package(glfw3 REQUIRED)

# Source files, including glad
set(SOURCE_FILES
    src/main.cpp
    src/glad.cpp
    src/Shader.cpp
)

include_directories(./include)

add_executable(main ${SOURCE_FILES})

target_link_libraries(main PRIVATE
    glfw
)
