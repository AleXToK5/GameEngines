#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <functional> // <--- ИНКЛУД
#include "../../GameEngine/Scene.h"
#include "../../GameEngine/GameEngine.h"
#include "../../Sample/Physics/MovementSystem.h"
#include "../../Sample/Physics/PlatformCollisionSystem.h"
#include "../../Sample/Camera/FollowXCameraSystem.h"
#include "../../Sample/Graphics/SpriteRenderSystem.h"
#include "../../Sample/Level/LevelInitializer.h"
#include "../../Sample/UI/UiSystem.h"
#include "../../Sample/Gameplay/Player/PlayerStateSystem.h"
#include "../../Sample/Graphics/AnimationSystem.h"
#include "../../Sample/Gameplay/Player/PlayerShootSystem.h"

class GameScene final : public Scene {
    static constexpr sf::Color BackgroundColor{0x64, 0x64, 0xff, 0xff};

    std::function<void()> _onQuit;
    bool _isExiting = false; // <--- ПРЕДОХРАНИТЕЛЬ

public:
    explicit GameScene(GameEngine &engine, std::function<void()> onQuit = nullptr)
        : Scene(engine), _onQuit(onQuit) {
    }

    void Init() override {
        systemsManager.AddInitializer(std::make_shared<LevelInitializer>(
            world, "level.json", "config.json", gameEngine.Assets(),
            static_cast<float>(gameEngine.Window().getSize().y)));

        RegisterAction(sf::Keyboard::Key::A, "MoveLeft");
        RegisterAction(sf::Keyboard::Key::D, "MoveRight");
        RegisterAction(sf::Keyboard::Key::W, "Jump");
        RegisterAction(sf::Keyboard::Key::Space, "Shoot");

        systemsManager.AddSystem(std::make_shared<PlayerShootSystem>(world, gameEngine.Assets(), actionMap["Shoot"]));
        systemsManager.AddSystem(
            std::make_shared<MovementSystem>(world, actionMap["MoveLeft"], actionMap["MoveRight"], actionMap["Jump"]));
        systemsManager.AddSystem(std::make_shared<PlatformCollisionSystem>(world));
        systemsManager.AddSystem(std::make_shared<FollowXCameraSystem>(world, gameEngine.Window()));
        systemsManager.AddSystem(std::make_shared<SpriteRenderSystem>(world, gameEngine.Window(), gameEngine.Assets()));
        systemsManager.AddSystem(std::make_shared<UiSystem>(world));
        systemsManager.AddSystem(std::make_shared<PlayerStateSystem>(world, gameEngine.Window().getSize().y));
        systemsManager.AddSystem(std::make_shared<AnimationSystem>(world, gameEngine.Assets()));

        systemsManager.Initialize();
    }

    void Update(float delta) override {
        if (_isExiting) return; // Блокируем, если уже выходим

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            _isExiting = true;
            if (_onQuit) _onQuit();
            return;
        }

        for (int e: FilterBuilder(world).With<PlayerComponent>().Build()) {
            if (world.GetStorage<PlayerComponent>().Get(e).IsFinished) {
                _isExiting = true;
                if (_onQuit) _onQuit();
                return;
            }
        }
    }

    void Render(sf::RenderWindow &window) override {
        window.clear(BackgroundColor);
        systemsManager.Update();
    }
};

#endif
