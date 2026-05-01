#ifndef ANIMATION_H
#define ANIMATION_H

#include "SFML/Graphics/Texture.hpp"

class Animation {
    const sf::Texture* _texture;  // указатель вместо ссылки
    size_t _frameCount;
    size_t _frameDuration;
    sf::Vector2i _size;

public:
    Animation(const sf::Texture& texture, size_t frameCount, size_t frameDuration);
    Animation(const Animation& other) = default;
    Animation(Animation&& other) = default;
    Animation& operator=(const Animation& other) = default;
    Animation& operator=(Animation&& other) = default;

    const sf::Texture& GetTexture() const { return *_texture; }
    size_t FrameCount() const { return _frameCount; }
    size_t FrameDuration() const { return _frameDuration; }
    const sf::Vector2i& Size() const { return _size; }
};

#endif //ANIMATION_H