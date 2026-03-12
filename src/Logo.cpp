#include "Logo.h"
#include <iostream>
#include <cmath>

Logo::Logo(const std::vector<std::filesystem::path>& paths,
           const sf::Vector2f& position,
           const sf::Vector2f& velocity,
           float scale,
           const sf::Color& color)
    : _velocity(velocity), _startPosition(position), _scale(scale)
{
    _textures.resize(paths.size());
    for (int i = 0; i < (int)paths.size(); i++)
    {
        if (!_textures[i].loadFromFile(paths[i])) {
            std::cerr << "Failed to load logo: " << paths[i] << "\n";
            exit(-1);
        }
    }

    _sprite.emplace(_textures[0]);
    _sprite->setPosition(position);
    _sprite->setScale({scale, scale});
    _sprite->setColor(color);
}

void Logo::SetTextureIndex(int index)
{
    if (index < 0 || index >= (int)_textures.size()) return;
    _currentIndex = index;

    // Запоминаем центр спрайта до смены текстуры
    auto oldBounds = _sprite->getGlobalBounds();
    sf::Vector2f oldCenter = {
        oldBounds.position.x + oldBounds.size.x / 2.f,
        oldBounds.position.y + oldBounds.size.y / 2.f
    };

    _sprite->setTexture(_textures[index]);

    // Пересчитываем позицию так чтобы центр остался на месте
    auto newBounds = _sprite->getGlobalBounds();
    _sprite->setPosition({
        oldCenter.x - newBounds.size.x / 2.f,
        oldCenter.y - newBounds.size.y / 2.f
    });
}

void Logo::Update(float deltaTime, const sf::Vector2u& windowSize)
{
    _sprite->move(_velocity * deltaTime);

    auto pos  = _sprite->getPosition();
    auto size = _sprite->getGlobalBounds().size;

    const float W = (float)windowSize.x;
    const float H = (float)windowSize.y;

    if (pos.x < 0.f)             _velocity.x =  std::abs(_velocity.x);
    else if (pos.x + size.x > W) _velocity.x = -std::abs(_velocity.x);

    if (pos.y < 0.f)             _velocity.y =  std::abs(_velocity.y);
    else if (pos.y + size.y > H) _velocity.y = -std::abs(_velocity.y);
}

void Logo::Draw(sf::RenderTarget& target)
{
    target.draw(*_sprite);
}