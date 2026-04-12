cmake_minimum_required(VERSION 4.1)
project(Demo
    VERSION 1.0
    LANGUAGES CXX)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
add_subdirectory(dependencies)
add_subdirectory(src)