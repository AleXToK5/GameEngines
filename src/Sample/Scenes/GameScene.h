#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../../GameEngine/Scene.h"
#include "../../GameEngine/GameEngine.h"
#include "../../Sample/Camera/FollowXCameraSystem.h"
#include "../../Sample/Graphics/SpriteRenderSystem.h"
#include "../../Sample/Level/LevelInitializer.h"

class GameScene final : public Scene {
    static constexpr sf::Color BackgroundColor{0x64, 0x64, 0xff, 0xff}; // #6464ff

public:
    explicit GameScene(GameEngine &engine)
        : Scene(engine) {
    }

    void Init() override {
        systemsManager.AddInitializer(
            std::make_shared<LevelInitializer>(world, "level.json", gameEngine.Assets()));

        systemsManager.AddSystem(
            std::make_shared<FollowXCameraSystem>(world, gameEngine.Window()));

        systemsManager.AddSystem(
            std::make_shared<SpriteRenderSystem>(world, gameEngine.Window(), gameEngine.Assets()));

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
