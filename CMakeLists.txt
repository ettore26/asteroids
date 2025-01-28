cmake_minimum_required(VERSION 3.10)
project(SimpleOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

# Add executable
add_executable(${PROJECT_NAME} 
    src/main.cpp
)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    OpenGL::GL
    GLEW::GLEW
    glfw
)
