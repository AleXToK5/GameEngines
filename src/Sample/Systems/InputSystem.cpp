#include "InputSystem.h"

void InputSystem::OnUpdate() {
    while (const std::optional event = _window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            _window.close();
        }
    }

    sf::Vector2f direction(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) direction.x = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) direction.x = 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) direction.y = -1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) direction.y = 1.f;

    for (const auto ent: _playerFilter) {
        auto &movement = _movements.Get(ent);
        movement.Direction = direction;
    }
}
