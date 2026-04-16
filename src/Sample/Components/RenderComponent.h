#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include <SFML/Graphics.hpp>

enum class ShapeType { Circle, Rectangle, Polygon };

struct RenderComponent {
    ShapeType Type;
    float Radius; // Для астероидов и игрока
    int PointCount; // Количество углов (3 - треугольник, 5-10 - астероид)
    sf::Vector2f Size; // Для прямоугольников (снарядов)
    sf::Color Color;
};

#endif //RENDERCOMPONENT_H
