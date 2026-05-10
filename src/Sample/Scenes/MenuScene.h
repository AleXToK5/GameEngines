#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "../../GameEngine/Scene.h"
#include "../../GameEngine/GameEngine.h"
#include "../../Sample/Camera/DefaultCameraSystem.h"
#include "GameScene.h"

class MenuScene final : public Scene {
    sf::Text _titleText;
    sf::Text _playText;
    sf::Text _exitText;

    bool _playHovered = false;
    bool _exitHovered = false;

    std::shared_ptr<InputAction> _mouseClick;
    std::shared_ptr<InputAction> _mouseMove;

public:
    explicit MenuScene(GameEngine &engine)
        : Scene(engine),
          _titleText(engine.Assets().GetFont("BaseFont"), ""),
          _playText(engine.Assets().GetFont("BaseFont"), ""),
          _exitText(engine.Assets().GetFont("BaseFont"), "") {
    }

    void Init() override {
        // Загружаем шрифт
        const sf::Font &font = gameEngine.Assets().GetFont("BaseFont");

        _titleText = sf::Text(font, "SUPER MARIO", 72);
        _titleText.setFillColor(sf::Color::Yellow);

        _playText = sf::Text(font, "PLAY", 48);
        _playText.setFillColor(sf::Color::White);

        _exitText = sf::Text(font, "EXIT", 48);
        _exitText.setFillColor(sf::Color::White);

        // Регистрируем ввод
        _mouseClick = std::make_shared<InputAction>("Click");
        gameEngine.RegisterInput(sf::Mouse::Button::Left, _mouseClick);

        _mouseMove = std::make_shared<InputAction>("MouseMove");
        gameEngine.RegisterInput(MouseMove::Move, _mouseMove);

        // Камера по умолчанию
        systemsManager.AddSystem(
            std::make_shared<DefaultCameraSystem>(world, gameEngine.Window()));

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

        centerText(_titleText, cx, cy - 150.f);
        centerText(_playText, cx, cy);
        centerText(_exitText, cx, cy + 100.f);

        if (_mouseMove->Type() == ActionType::Start) {
            sf::Vector2f mp = static_cast<sf::Vector2f>(_mouseMove->Value2());
            _playHovered = _playText.getGlobalBounds().contains(mp);
            _exitHovered = _exitText.getGlobalBounds().contains(mp);
            _playText.setFillColor(_playHovered ? sf::Color::Yellow : sf::Color::White);
            _exitText.setFillColor(_exitHovered ? sf::Color::Yellow : sf::Color::White);
        }

        if (_mouseClick->Type() == ActionType::End) {
            sf::Vector2f mp = static_cast<sf::Vector2f>(_mouseMove->Value2());
            if (_playText.getGlobalBounds().contains(mp)) {
                auto &eng = gameEngine;
                eng.RequestSceneChange([&eng]() {
                    // Передаем коллбек возврата в меню
                    eng.LoadScene<GameScene>(eng, [&eng]() {
                        eng.RequestSceneChange([&eng]() {
                            eng.LoadScene<MenuScene>(eng);
                        });
                    });
                });
            } else if (_exitText.getGlobalBounds().contains(mp)) {
                gameEngine.Quit();
            }
        }

        systemsManager.Update();
    }

    void Render(sf::RenderWindow &window) override {
        window.clear(sf::Color::Black);
        window.draw(_titleText);
        window.draw(_playText);
        window.draw(_exitText);
    }
};

#endif //MENUSCENE_H
