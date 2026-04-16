#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <SFML/System/Vector2.hpp>

enum class ColliderType { Circle, AABB };

struct ColliderComponent {
    ColliderType Type;
    float Radius; // Для кругов
    sf::Vector2f Size; // Для AABB
};

#endif //COLLIDERCOMPONENT_H
