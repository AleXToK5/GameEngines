#include "MovementSystem.h"

void MovementSystem::Print(int ent)
{
    auto& position = _positionComponents.Get(ent);
    std::cout << ent << " Pos: " << position.X << ", " << position.Y << std::endl;
}

void MovementSystem::OnInit()
{
}

void MovementSystem::OnUpdate()
{
    for (const auto event : _moveInputEvents)
    {
        auto& inputEvent = _eventComponents.Get(event);

        for (const auto ent : _moveables)
        {
            auto& position = _positionComponents.Get(ent);
            auto& movement = _movementComponents.Get(ent);

            // Применяем направление из события ввода
            movement.Direction = inputEvent.Direction;

            position.X += movement.Speed * movement.Direction.x;
            position.Y += movement.Speed * movement.Direction.y;

            Print(ent);
        }

        world.RemoveEntity(event);
    }
}