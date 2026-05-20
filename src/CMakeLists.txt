set(TARGET_NAME main)
set(SOURCES main.cpp
        Sample/Physics/MovementSystem.h
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
        GameEngine/Serialization/ComponentSerializer.cpp
        GameEngine/Serialization/ComponentSerializer.h
        GameEngine/Serialization/ComponentSerializerFuncs.h
        GameEngine/Serialization/ComponentsMetadata.h
        GameEngine/Serialization/IComponentSerializer.h
        GameEngine/Serialization/SerializedEntity.h
        GameEngine/Editor/EditorScene.cpp
        GameEngine/Editor/EditorScene.h
        GameEngine/Editor/CameraService.h
        GameEngine/Editor/Constants.h
        GameEngine/Editor/GameObjectController.h
        GameEngine/Editor/GameObjectFactory.h
        GameEngine/Editor/JsonSerializer.h
        GameEngine/Editor/RenderService.h
        GameEngine/Editor/RenderState.h
        GameEngine/Editor/Systems/EntityGuiSystem.cpp
        GameEngine/Editor/Systems/EntityGuiSystem.h
        GameEngine/Editor/Systems/GuiSystem.cpp
        GameEngine/Editor/Systems/GuiSystem.h
        GameEngine/Editor/Systems/InputSystem.cpp
        GameEngine/Editor/Systems/InputSystem.h
        GameEngine/Editor/Systems/MouseInputSystem.cpp
        GameEngine/Editor/Systems/MouseInputSystem.h
        GameEngine/Editor/Systems/RenderSystem.cpp
        GameEngine/Editor/Systems/RenderSystem.h
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Assets
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(Ecs)

add_executable(${TARGET_NAME}
        ${SOURCES})

#target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)
target_compile_definitions(${TARGET_NAME} PRIVATE EDITOR_ENABLED)
target_link_libraries(${TARGET_NAME} PUBLIC ${ECS_LIB_NAME})
target_link_libraries(${TARGET_NAME} PUBLIC ImGui-SFML::ImGui-SFML
        nlohmann_json::nlohmann_json)