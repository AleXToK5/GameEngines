#ifndef MENUSCENE_H
#define MENUSCENE_H

#include <fstream>
#include "../../GameEngine/Scene.h"
#include "../../GameEngine/GameEngine.h"
#include "../../Sample/Camera/DefaultCameraSystem.h"
#include "../../Sample/Level/LevelInitializer.h"
#include "../../Sample/Graphics/SpriteRenderSystem.h"
#include "../../Sample/Graphics/AnimationSystem.h"
#include "../../Sample/Graphics/RenderSettingsComponent.h"
#include "GameScene.h"

class MenuScene final : public Scene {
    sf::RectangleShape _dimOverlay;

    sf::RectangleShape _playBtnBg;
    sf::RectangleShape _exitBtnBg;

    sf::Text _titleText;
    sf::Text _playText;
    sf::Text _exitText;

    sf::Text _bestTimeText;

    bool _playHovered = false;
    bool _exitHovered = false;

    std::shared_ptr<InputAction> _mouseClick;
    std::shared_ptr<InputAction> _mouseMove;

public:
    explicit MenuScene(GameEngine &engine)
        : Scene(engine),
          _titleText(engine.Assets().GetFont("BaseFont"), ""),
          _playText(engine.Assets().GetFont("BaseFont"), ""),
          _exitText(engine.Assets().GetFont("BaseFont"), ""),
          _bestTimeText(engine.Assets().GetFont("BaseFont"), "") {
    }

    void Init() override {
        systemsManager.AddInitializer(
            std::make_shared<LevelInitializer>(
                world, "level.json", "config.json", gameEngine.Assets(),
                static_cast<float>(gameEngine.Window().getSize().y)
            )
        );

        int settingsEnt = world.CreateEntity();
        world.GetStorage<RenderSettingsComponent>().Add(settingsEnt, {true, false, false});

        systemsManager.AddSystem(std::make_shared<SpriteRenderSystem>(world, gameEngine.Window(), gameEngine.Assets()));
        systemsManager.AddSystem(std::make_shared<AnimationSystem>(world, gameEngine.Assets()));
        systemsManager.AddSystem(std::make_shared<DefaultCameraSystem>(world, gameEngine.Window()));

        _dimOverlay.setSize({
            static_cast<float>(gameEngine.Window().getSize().x),
            static_cast<float>(gameEngine.Window().getSize().y)
        });
        _dimOverlay.setFillColor(sf::Color(0, 0, 0, 160));

        const sf::Font &font = gameEngine.Assets().GetFont("BaseFont");

        _titleText = sf::Text(font, "SUPER MEGA MARIO", 72);
        _titleText.setFillColor(sf::Color(255, 215, 0));
        _titleText.setOutlineColor(sf::Color::Black);
        _titleText.setOutlineThickness(5.0f);

        _playText = sf::Text(font, "PLAY", 48);
        _playText.setFillColor(sf::Color::White);

        _playBtnBg.setSize({300.f, 80.f});
        _playBtnBg.setFillColor(sf::Color(50, 50, 50, 220));
        _playBtnBg.setOutlineColor(sf::Color::White);
        _playBtnBg.setOutlineThickness(2.f);

        _exitText = sf::Text(font, "EXIT", 48);
        _exitText.setFillColor(sf::Color::White);

        _exitBtnBg.setSize({300.f, 80.f});
        _exitBtnBg.setFillColor(sf::Color(50, 50, 50, 220));
        _exitBtnBg.setOutlineColor(sf::Color::White);
        _exitBtnBg.setOutlineThickness(2.f);

        float bestTime = 0.f;
        std::ifstream inFile("best_time.txt");
        if (inFile.is_open()) {
            inFile >> bestTime;
        }

        char timeStr[64];
        if (bestTime > 0.f) {
            snprintf(timeStr, sizeof(timeStr), "BEST TIME: %.1f s", bestTime);
        } else {
            snprintf(timeStr, sizeof(timeStr), "BEST TIME: --");
        }

        _bestTimeText = sf::Text(font, timeStr, 32);
        _bestTimeText.setFillColor(sf::Color::Cyan);
        _bestTimeText.setOutlineColor(sf::Color::Black);
        _bestTimeText.setOutlineThickness(2.f);

        _mouseClick = std::make_shared<InputAction>("Click");
        gameEngine.RegisterInput(sf::Mouse::Button::Left, _mouseClick);

        _mouseMove = std::make_shared<InputAction>("MouseMove");
        gameEngine.RegisterInput(Move, _mouseMove);

        systemsManager.Initialize();
    }

    void Update(float delta) override {
        auto &win = gameEngine.Window();
        float cx = win.getSize().x / 2.f;
        float cy = win.getSize().y / 2.f;

        auto centerText = [](sf::Text &t, float x, float y) {
            sf::FloatRect b = t.getLocalBounds();
            t.setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
            t.setPosition({x, y});
        };
        auto centerRect = [](sf::RectangleShape &r, float x, float y) {
            r.setOrigin({r.getSize().x / 2.f, r.getSize().y / 2.f});
            r.setPosition({x, y});
        };

        centerText(_titleText, cx, cy - 200.f);

        centerRect(_playBtnBg, cx, cy - 20.f);
        centerText(_playText, cx, cy - 25.f);

        centerRect(_exitBtnBg, cx, cy + 100.f);
        centerText(_exitText, cx, cy + 95.f);

        centerText(_bestTimeText, cx, cy + 180.f);

        if (_mouseMove->Type() == ActionType::Start) {
            sf::Vector2f mp = static_cast<sf::Vector2f>(_mouseMove->Value2());

            _playHovered = _playBtnBg.getGlobalBounds().contains(mp);
            _exitHovered = _exitBtnBg.getGlobalBounds().contains(mp);

            _playBtnBg.setFillColor(_playHovered ? sf::Color(70, 130, 200, 255) : sf::Color(50, 50, 50, 220));
            _playText.setFillColor(_playHovered ? sf::Color::Yellow : sf::Color::White);

            _exitBtnBg.setFillColor(_exitHovered ? sf::Color(200, 70, 70, 255) : sf::Color(50, 50, 50, 220));
            _exitText.setFillColor(_exitHovered ? sf::Color::Yellow : sf::Color::White);
        }

        if (_mouseClick->Type() == ActionType::End) {
            sf::Vector2f mp = static_cast<sf::Vector2f>(_mouseMove->Value2());

            if (_playBtnBg.getGlobalBounds().contains(mp)) {
                auto &eng = gameEngine;
                eng.RequestSceneChange([&eng]() {
                    eng.LoadScene<GameScene>(eng, [&eng]() {
                        eng.RequestSceneChange([&eng]() {
                            eng.LoadScene<MenuScene>(eng);
                        });
                    });
                });
            } else if (_exitBtnBg.getGlobalBounds().contains(mp)) {
                gameEngine.Quit();
            }
        }
    }

    void Render(sf::RenderWindow &window) override {
        window.clear(sf::Color(100, 100, 255));

        systemsManager.Update();
        window.draw(_dimOverlay);

        window.draw(_titleText);
        window.draw(_playBtnBg);
        window.draw(_playText);
        window.draw(_exitBtnBg);
        window.draw(_exitText);
        window.draw(_bestTimeText);
    }
};

#endif
