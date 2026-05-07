#include "Animation.h"

Animation::Animation(const sf::Texture &texture, const size_t frameCount, const size_t frameDuration)
    : _texture(&texture), _frameCount(frameCount), _frameDuration(frameDuration) {
    _size.x = static_cast<int>(texture.getSize().x / frameCount);
    _size.y = static_cast<int>(texture.getSize().y);
}
