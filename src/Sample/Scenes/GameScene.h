#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <functional>
#include <fstream>
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
#include "../../Sample/Gameplay/Environment/AiSystem.h"

class GameScene final : public Scene {
    static constexpr sf::Color BackgroundColor{0x64, 0x64, 0xff, 0xff};

    std::function<void()> _onQuit;
    bool _isExiting = false;
    bool _isPaused = false;

    float _levelTime = 0.f;
    sf::Text _hudTimeText;
    sf::Text _pauseText;
    sf::RectangleShape _pauseDim;

public:
    explicit GameScene(GameEngine &engine, std::function<void()> onQuit = nullptr)
        : Scene(engine), _onQuit(onQuit),
          _hudTimeText(engine.Assets().GetFont("BaseFont")),
          _pauseText(engine.Assets().GetFont("BaseFont")) {
    }

    void Init() override {
        systemsManager.AddInitializer(std::make_shared<LevelInitializer>(
            world, "level.json", "config.json", gameEngine.Assets(),
            static_cast<float>(gameEngine.Window().getSize().y)));

        RegisterAction(sf::Keyboard::Key::A, "MoveLeft");
        RegisterAction(sf::Keyboard::Key::D, "MoveRight");
        RegisterAction(sf::Keyboard::Key::W, "Jump");
        RegisterAction(sf::Keyboard::Key::Space, "Shoot");
        RegisterAction(sf::Keyboard::Key::P, "Pause");

        systemsManager.AddSystem(std::make_shared<PlayerShootSystem>(world, gameEngine.Assets(), actionMap["Shoot"]));
        systemsManager.
                AddSystem(std::make_shared<AiSystem>(world, static_cast<float>(gameEngine.Window().getSize().y)));
        systemsManager.AddSystem(
            std::make_shared<MovementSystem>(world, actionMap["MoveLeft"], actionMap["MoveRight"], actionMap["Jump"]));
        systemsManager.AddSystem(std::make_shared<PlatformCollisionSystem>(world));
        systemsManager.AddSystem(std::make_shared<FollowXCameraSystem>(world, gameEngine.Window()));
        systemsManager.AddSystem(std::make_shared<SpriteRenderSystem>(world, gameEngine.Window(), gameEngine.Assets()));
        systemsManager.AddSystem(std::make_shared<UiSystem>(world));
        systemsManager.AddSystem(std::make_shared<PlayerStateSystem>(world, gameEngine.Window().getSize().y));
        systemsManager.AddSystem(std::make_shared<AnimationSystem>(world, gameEngine.Assets()));

        _hudTimeText = sf::Text(gameEngine.Assets().GetFont("BaseFont"), "TIME: 0.0", 36);
        _hudTimeText.setFillColor(sf::Color::White);
        _hudTimeText.setOutlineColor(sf::Color::Black);
        _hudTimeText.setOutlineThickness(3.f);
        _hudTimeText.setPosition({20.f, 20.f});

        _pauseText.setString("PAUSE");
        _pauseText.setCharacterSize(100);
        _pauseText.setFillColor(sf::Color::Yellow);
        _pauseText.setOutlineColor(sf::Color::Black);
        _pauseText.setOutlineThickness(5.f);

        sf::FloatRect bounds = _pauseText.getLocalBounds();
        _pauseText.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
        _pauseText.setPosition({gameEngine.Window().getSize().x / 2.f, gameEngine.Window().getSize().y / 2.f});

        _pauseDim.setSize(static_cast<sf::Vector2f>(gameEngine.Window().getSize()));
        _pauseDim.setFillColor(sf::Color(0, 0, 0, 150));

        systemsManager.Initialize();
    }

    void Update(float delta) override {
        if (_isExiting) return;

        if (actionMap["Pause"]->Type() == ActionType::Start) {
            _isPaused = !_isPaused;
            actionMap["Pause"]->Type() = ActionType::None;
        }

        if (_isPaused) return;

        _levelTime += delta;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            _isExiting = true;
            if (_onQuit) _onQuit();
            return;
        }

        for (int e: FilterBuilder(world).With<PlayerComponent>().Build()) {
            if (world.GetStorage<PlayerComponent>().Get(e).IsFinished) {
                _isExiting = true;

                float bestTime = 999999.f;
                std::ifstream inFile("best_time.txt");
                if (inFile.is_open()) {
                    inFile >> bestTime;
                    inFile.close();
                }

                if (_levelTime < bestTime) {
                    std::ofstream outFile("best_time.txt");
                    outFile << _levelTime;
                }

                if (_onQuit) _onQuit();
                return;
            }
        }
    }

    void Render(sf::RenderWindow &window) override {
        window.clear(BackgroundColor);

        if (!_isPaused) {
            systemsManager.Update();
        }

        char timeStr[32];
        snprintf(timeStr, sizeof(timeStr), "TIME: %.1f", _levelTime);
        _hudTimeText.setString(timeStr);

        sf::View oldView = window.getView();
        window.setView(window.getDefaultView());

        window.draw(_hudTimeText);

        if (_isPaused) {
            window.draw(_pauseDim);
            window.draw(_pauseText);
        }

        window.setView(oldView);
    }
};

#endif
