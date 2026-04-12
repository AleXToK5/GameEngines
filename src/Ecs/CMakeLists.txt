set(LIB_NAME easy_ecs)
set(ECS_LIB_NAME ${LIB_NAME} PARENT_SCOPE)
set(SOURCES World/World.cpp
        Components/ComponentStorage.hpp
        Filter/FilterBuilder.cpp
        Filter/Filter.cpp
        Entity/EntityId.cpp
        Systems/SystemsManager.cpp
)

add_library(${LIB_NAME}
    ${SOURCES})

target_compile_definitions(${LIB_NAME} PUBLIC -DDEBUG)
#target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_compile_features(${LIB_NAME} PRIVATE cxx_std_20)