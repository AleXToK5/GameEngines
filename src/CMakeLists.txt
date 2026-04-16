set(TARGET_NAME main)
set(SOURCES main.cpp
        ConfigReader.cpp
        ConfigReader.h
        Sample/Systems/MovementSystem.cpp
        Sample/Systems/MovementSystem.h
        Sample/Systems/InitSystem.cpp
        Sample/Systems/InitSystem.h
        Sample/Systems/RenderSystem.cpp
        Sample/Systems/RenderSystem.h
        Sample/Components/PositionComponent.h
        Sample/Components/MovementComponent.h
        Sample/Components/RenderComponent.h
        Sample/Systems/InputSystem.cpp
        Sample/Systems/InputSystem.h
        Sample/Components/MoveInputEvent.h
        Sample/Systems/PlayerShootSystem.cpp
        Sample/Systems/AsteroidSpawnerSystem.cpp
        Sample/Systems/CollisionSystem.cpp
        Sample/Systems/RestartSystem.cpp
        Sample/Systems/UiSystem.cpp
)

add_subdirectory(Ecs)

add_executable(${TARGET_NAME} 
    ${SOURCES})

#target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

target_link_libraries(${TARGET_NAME} PUBLIC ${ECS_LIB_NAME})
target_link_libraries(${TARGET_NAME} PUBLIC ImGui-SFML::ImGui-SFML
        nlohmann_json::nlohmann_json)