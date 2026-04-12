#ifndef MOVEMENTCOMPONENT_H
#define MOVEMENTCOMPONENT_H

#include <SFML/System/Vector2.hpp>

struct MovementComponent
{
    float Speed;
    sf::Vector2f Direction;
};

#endif //MOVEMENTCOMPONENT_H