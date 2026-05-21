#ifndef GOOMBACOMPONENT_H
#define GOOMBACOMPONENT_H

#include <vector>
#include <SFML/System/Vector2.hpp>

enum class GoombaState { Patrol, Pursue };

struct GoombaComponent {
    GoombaState State = GoombaState::Patrol;
    float PatrolLeftX = 0.f;
    float PatrolRightX = 0.f;
    float SpeedX = 2.0f;
    int Direction = -1;

    std::vector<sf::Vector2i> CurrentPath;
    bool IsJumping = false;
    sf::Vector2f JumpStart;
    sf::Vector2f JumpTarget;
    float JumpTime = 0.f;
};

#endif // GOOMBACOMPONENT_H
