cmake_minimum_required(VERSION 4.1)
project(Demo
    VERSION 1.0
    LANGUAGES CXX)
file(COPY
        config.json
        assets.json
        level.json
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/src)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
add_subdirectory(dependencies)
add_subdirectory(src)