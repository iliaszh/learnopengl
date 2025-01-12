cmake_minimum_required(VERSION 3.30)

project(
        learn-opengl
        VERSION 0.1.0
)

add_library(compile_requirements INTERFACE)

target_compile_features(
        compile_requirements
        INTERFACE
        cxx_std_20
)

add_executable(
        game
        src/main.cpp
        deps/glad/src/glad.c
)

add_subdirectory(deps/glfw)

target_link_libraries(
        game
        PRIVATE
        compile_requirements
        glfw
)

target_include_directories(
        game
        PRIVATE
        include
        deps/glfw/include
        deps/glad/include
)
