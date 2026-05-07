#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H
#include <string>
#include <unordered_map>

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Texture.hpp"

#include "Animation.h"

class AssetManager {
    std::unordered_map<std::string, std::unique_ptr<sf::Texture> > _textures;
    std::unordered_map<std::string, Animation> _animations;
    std::unordered_map<std::string, sf::Font> _fonts;

public:
    void LoadFromFile(const std::string &path);

    void AddTexture(const std::string &name, const std::string &path);

    void AddAnimation(const std::string &name, const std::string &textureName, size_t frameCount, size_t frameDuration);

    void AddFont(const std::string &name, const std::string &path);

    const sf::Texture &GetTexture(const std::string &name) const;

    const Animation &GetAnimation(const std::string &name) const;

    const sf::Font &GetFont(const std::string &name) const;

    const std::unordered_map<std::string, std::unique_ptr<sf::Texture> > &GetTextures() const { return _textures; }
    const std::unordered_map<std::string, Animation> &GetAnimations() const { return _animations; }
};

#endif //ASSETMANAGER_H
