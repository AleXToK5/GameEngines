
#ifndef DEMO_OBJECT_H
#define DEMO_OBJECT_H
#include <SFML/Graphics.hpp>

class Object {
public:
    virtual ~Object() = default;

    virtual void Update(float deltaTime, const sf::Vector2u& windowSize) = 0;
    virtual void Draw(sf::RenderTarget& target) = 0;
};


#endif //DEMO_OBJECT_H