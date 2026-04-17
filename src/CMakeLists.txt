set(TARGET_NAME main)
set(SOURCES main.cpp
        ConfigReader.cpp
        ConfigReader.h
        Sample/Physics/MovementSystem.cpp
        Sample/Physics/MovementSystem.h
        Sample/Core/InitSystem.cpp
        Sample/Core/InitSystem.h
        Sample/Graphics/RenderSystem.cpp
        Sample/Graphics/RenderSystem.h
        Sample/Physics/PositionComponent.h
        Sample/Physics/MovementComponent.h
        Sample/Graphics/RenderComponent.h
        Sample/Input/InputSystem.cpp
        Sample/Input/InputSystem.h
        Sample/Input/MoveInputEvent.h
        Sample/Gameplay/Player/PlayerShootSystem.cpp
        Sample/Gameplay/Enemies/AsteroidSpawnerSystem.cpp
        Sample/Physics/CollisionSystem.cpp
        Sample/Core/RestartSystem.cpp
        Sample/UI/UiSystem.cpp
)

add_subdirectory(Ecs)

add_executable(${TARGET_NAME} 
    ${SOURCES})

#target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

target_link_libraries(${TARGET_NAME} PUBLIC ${ECS_LIB_NAME})
target_link_libraries(${TARGET_NAME} PUBLIC ImGui-SFML::ImGui-SFML
        nlohmann_json::nlohmann_json)