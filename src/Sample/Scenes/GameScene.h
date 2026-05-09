#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../../GameEngine/Scene.h"
#include "../../GameEngine/GameEngine.h"
#include "../../Sample/Physics/MovementSystem.h"
#include "../../Sample/Physics/PlatformCollisionSystem.h"
#include "../../Sample/Camera/FollowXCameraSystem.h"
#include "../../Sample/Graphics/SpriteRenderSystem.h"
#include "../../Sample/Level/LevelInitializer.h"
#include "../../Sample/UI/UiSystem.h"

class GameScene final : public Scene {
    static constexpr sf::Color BackgroundColor{0x64, 0x64, 0xff, 0xff}; // #6464ff

public:
    explicit GameScene(GameEngine &engine)
        : Scene(engine) {
    }

    void Init() override {
        systemsManager.AddInitializer(
            std::make_shared<LevelInitializer>(
                world,
                "level.json",
                "config.json",
                gameEngine.Assets(),
                static_cast<float>(gameEngine.Window().getSize().y)
            )
        );

        RegisterAction(sf::Keyboard::Key::A, "MoveLeft");
        RegisterAction(sf::Keyboard::Key::D, "MoveRight");
        RegisterAction(sf::Keyboard::Key::W, "Jump");

        systemsManager.AddSystem(std::make_shared<MovementSystem>(
            world,
            actionMap["MoveLeft"],
            actionMap["MoveRight"],
            actionMap["Jump"]
        ));

        systemsManager.AddSystem(std::make_shared<PlatformCollisionSystem>(world));

        systemsManager.AddSystem(std::make_shared<FollowXCameraSystem>(world, gameEngine.Window()));
        systemsManager.AddSystem(std::make_shared<SpriteRenderSystem>(world, gameEngine.Window(), gameEngine.Assets()));
        systemsManager.AddSystem(std::make_shared<UiSystem>(world));

        systemsManager.Initialize();
    }

    void Update(float delta) override {
        ;
    }

    void Render(sf::RenderWindow &window) override {
        window.clear(BackgroundColor);
        systemsManager.Update();
    }
};

#endif //GAMESCENE_H
