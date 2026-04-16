#include "RenderSystem.h"
#include <iostream>
#include <string>

void RenderSystem::OnInit() {
}

void RenderSystem::OnUpdate() {
    int entityCount = 0;
    for (const auto ent: _renderables) {
        auto &position = _positionComponents.Get(ent);
        auto &render = _renderComponents.Get(ent);

        if (render.Type == ShapeType::Rectangle) {
            sf::RectangleShape rect(render.Size);
            rect.setOrigin({render.Size.x / 2.f, render.Size.y / 2.f});
            rect.setPosition({position.X, position.Y});
            rect.setFillColor(render.Color);
            _window.draw(rect);
        } else {
            sf::CircleShape shape(render.Radius, render.PointCount);
            shape.setOrigin({render.Radius, render.Radius});
            shape.setPosition({position.X, position.Y});
            shape.setFillColor(render.Color);
            _window.draw(shape);
        }
        entityCount++;
    }

    int currentScore = 0;
    bool isGameOver = false;

    for (int ent: _gameStateFilter) {
        auto &state = _gameStates.Get(ent);
        currentScore = state.Score;
        isGameOver = state.IsGameOver;
        break;
    }

    sf::Text text(_font, "Score: " + std::to_string(currentScore) +
                         "\nEntities: " + std::to_string(entityCount) +
                         "\nControls: Arrows to move, Space to shoot", 20);
    text.setFillColor(sf::Color::White);
    text.setPosition({10.f, 10.f});
    _window.draw(text);

    if (isGameOver) {
        // Надпись GAME OVER
        sf::Text gameOverText(_font, "GAME OVER", 60);
        gameOverText.setFillColor(sf::Color::Red);
        sf::FloatRect textRect = gameOverText.getLocalBounds();
        gameOverText.setOrigin({
            textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f
        });
        gameOverText.setPosition({_window.getSize().x / 2.0f, _window.getSize().y / 2.0f - 30.f});
        _window.draw(gameOverText);

        // Финальный счет
        sf::Text finalScoreText(_font, "Final Score: " + std::to_string(currentScore), 30);
        finalScoreText.setFillColor(sf::Color::Yellow);
        sf::FloatRect fsRect = finalScoreText.getLocalBounds();
        finalScoreText.setOrigin({fsRect.position.x + fsRect.size.x / 2.0f, fsRect.position.y + fsRect.size.y / 2.0f});
        finalScoreText.setPosition({_window.getSize().x / 2.0f, _window.getSize().y / 2.0f + 100.f});
        _window.draw(finalScoreText);

        // Подсказка для рестарта
        sf::Text restartText(_font, "Press 'R' to Restart", 30);
        restartText.setFillColor(sf::Color::White);
        sf::FloatRect restartRect = restartText.getLocalBounds();
        restartText.setOrigin({
            restartRect.position.x + restartRect.size.x / 2.0f, restartRect.position.y + restartRect.size.y / 2.0f
        });
        restartText.setPosition({_window.getSize().x / 2.0f, _window.getSize().y / 2.0f + 40.f});
        _window.draw(restartText);
    }
}
