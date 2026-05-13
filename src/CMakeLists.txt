set(TARGET_NAME main)
set(SOURCES main.cpp
        ConfigReader.cpp
        ConfigReader.h
        Sample/Physics/MovementSystem.h
        Sample/Physics/PositionComponent.h
        Sample/Physics/MovementComponent.h
        Sample/UI/UiSystem.cpp
        GameEngine/GameEngine.cpp
        GameEngine/GameEngine.h
        GameEngine/GameEngineConfiguration.h
        GameEngine/Scene.cpp
        GameEngine/Scene.h
        GameEngine/Assets/Animation.cpp
        GameEngine/Assets/Animation.h
        GameEngine/Assets/AssetManager.cpp
        GameEngine/Assets/AssetManager.h
        GameEngine/Input/InputAction.cpp
        GameEngine/Input/InputAction.h
        GameEngine/Input/InputManager.cpp
        GameEngine/Input/InputManager.h
        Sample/Scenes/MenuScene.h
        Sample/Scenes/GameScene.h
        Sample/Camera/CameraComponent.h
        Sample/Camera/DefaultCameraTag.h
        Sample/Camera/FollowXCameraTag.h
        Sample/Camera/DefaultCameraSystem.h
        Sample/Camera/FollowXCameraSystem.h
        Sample/Graphics/SpriteRenderSystem.h
        Sample/Graphics/SpriteComponent.h
        Sample/Level/LevelInitializer.h
        Sample/Gameplay/PlayerTag.h
        Sample/Physics/TransformComponent.h
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(Ecs)

add_executable(${TARGET_NAME}
        ${SOURCES})

#target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

target_link_libraries(${TARGET_NAME} PUBLIC ${ECS_LIB_NAME})
target_link_libraries(${TARGET_NAME} PUBLIC ImGui-SFML::ImGui-SFML
        nlohmann_json::nlohmann_json)