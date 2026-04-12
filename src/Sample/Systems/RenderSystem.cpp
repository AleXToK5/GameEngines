#include "RenderSystem.h"
#include <iostream>

void RenderSystem::OnInit()
{
}

void RenderSystem::OnUpdate()
{
    _window.clear(sf::Color::Black);

    // Рисуем все сущности с компонентами позиции и рендера
    int entityCount = 0;
    for (const auto ent : _renderables)
    {
        auto& position = _positionComponents.Get(ent);
        auto& render = _renderComponents.Get(ent);

        sf::CircleShape circle(render.Radius);
        circle.setPosition(sf::Vector2f(position.X, position.Y));
        circle.setFillColor(render.Color);
        _window.draw(circle);

        entityCount++;
    }

    // Выводим информацию на экран
    sf::Text text(_font, "Entities with render: " + std::to_string(entityCount) + "\nControls: Arrow Keys to move", 20);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(10.f, 10.f));
    _window.draw(text);

    _window.display();
}
