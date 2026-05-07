#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <SFML/System/Vector2.hpp>

struct TransformComponent {
    float X;
    float Y;
    float ScaleX = 1.f;
    float ScaleY = 1.f;
};

#endif //TRANSFORMCOMPONENT_H
