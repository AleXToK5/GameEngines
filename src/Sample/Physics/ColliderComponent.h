#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include <SFML/System/Vector2.hpp>
#include <cstdint>

enum class ColliderType { Circle, AABB };

enum CollisionLayer : uint16_t {
    None = 0,
    Player = 1 << 0,
    Projectile = 1 << 1,
    Tile = 1 << 2,
    Enemy = 1 << 3
};

struct ColliderComponent {
    ColliderType Type;
    float Radius; // Для кругов
    sf::Vector2f Size; // Для AABB

    // Настройки фильтрации
    uint16_t Layer = CollisionLayer::None; // Обьект
    uint16_t Mask = CollisionLayer::None; // С кем сталкивается
};

#endif //COLLIDERCOMPONENT_H
