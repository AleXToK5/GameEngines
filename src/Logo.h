#ifndef DEMO_LOGO_H
#define DEMO_LOGO_H
#include "Object.h"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <optional>
#include <vector>

class Logo : public Object {
    std::vector<sf::Texture> _textures;
    std::optional<sf::Sprite> _sprite;
    sf::Vector2f _velocity;
    sf::Vector2f _startPosition;
    float        _scale;
    int          _currentIndex = 0;

public:
    Logo(const std::vector<std::filesystem::path>& paths,
         const sf::Vector2f& position,
         const sf::Vector2f& velocity,
         float scale,
         const sf::Color& color);

    void Update(float deltaTime, const sf::Vector2u& windowSize) override;
    void Draw(sf::RenderTarget& target) override;

    void SetTextureIndex(int index);
    int  GetTextureIndex() const { return _currentIndex; }
    int  GetTextureCount() const { return (int)_textures.size(); }

    sf::Vector2f GetVelocity() const         { return _velocity; }
    void         SetVelocity(sf::Vector2f v) { _velocity = v; }

    float GetScale() const  { return _scale; }
    void  SetScale(float s) { _scale = s; _sprite->setScale({s, s}); }

    sf::Color GetColor() const      { return _sprite->getColor(); }
    void      SetColor(sf::Color c) { _sprite->setColor(c); }

    void ResetPosition() { _sprite->setPosition(_startPosition); }
};

#endif //DEMO_LOGO_H