#include "InputSystem.h"

void InputSystem::OnInit()
{
}

void InputSystem::OnUpdate()
{
    while (const std::optional event = _window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            _window.close();
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            sf::Vector2f direction(0.f, 0.f);

            if (keyPressed->code == sf::Keyboard::Key::Left)
                direction.x = -1.f;
            else if (keyPressed->code == sf::Keyboard::Key::Right)
                direction.x = 1.f;
            else if (keyPressed->code == sf::Keyboard::Key::Up)
                direction.y = -1.f;
            else if (keyPressed->code == sf::Keyboard::Key::Down)
                direction.y = 1.f;

            const int eventEnt = world.CreateEntity();
            _eventComponents.Add(eventEnt, MoveInputEvent{direction});
        }
    }
}