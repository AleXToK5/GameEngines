cmake_minimum_required(VERSION 4.1)
project(Demo
    VERSION 1.0
    LANGUAGES CXX)

file(COPY
    fonts/futura.ttf
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/fonts)
#file(COPY
#    config.txt
#    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(dependencies)
add_subdirectory(src)