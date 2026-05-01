#include "AssetManager.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void AssetManager::LoadFromFile(const std::string &path)
{

    std::cout << "Current working dir: " << std::filesystem::current_path() << std::endl;
    std::cout << "Looking for: " << path << std::endl;
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Failed to open asset file: " << path << std::endl;
        return;
    }

    json data;
    try
    {
        file >> data;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed to parse JSON: " << e.what() << std::endl;
        return;
    }

    // Load textures
    if (data.contains("Textures"))
    {
        for (const auto& texture : data["Textures"])
        {
            if (texture.contains("name") && texture.contains("path"))
            {
                AddTexture(texture["name"], texture["path"]);
            }
        }
    }

    // Load animations
    if (data.contains("Animations"))
    {
        for (const auto& anim : data["Animations"])
        {
            if (anim.contains("name") && anim.contains("textureName") && 
                anim.contains("frameCount") && anim.contains("frameDuration"))
            {
                AddAnimation(anim["name"], anim["textureName"], 
                           anim["frameCount"], anim["frameDuration"]);
            }
        }
    }

    // Load fonts
    if (data.contains("Fonts"))
    {
        for (const auto& font : data["Fonts"])
        {
            if (font.contains("name") && font.contains("path"))
            {
                AddFont(font["name"], font["path"]);
            }
        }
    }
}

void AssetManager::AddTexture(const std::string& name, const std::string& path)
{
    auto texture = std::make_unique<sf::Texture>();
    if (texture->loadFromFile(path))
    {
        std::cout << "Loaded texture: " << name << " from " << path << std::endl;
        _textures[name] = std::move(texture);
    }
    else
    {
        std::cerr << "Failed to load texture: " << name << " from " << path << std::endl;
    }
}

void AssetManager::AddAnimation(const std::string& name, const std::string& textureName,
    const size_t frameCount, const size_t frameDuration)
{
    if (!_textures.count(textureName))
    {
        std::cerr << "Texture not found for animation: " << name << std::endl;
        return;
    }

    // *_textures[textureName] — указатель стабилен, unique_ptr не перемещается при rehash
    _animations.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(name),
        std::forward_as_tuple(*_textures[textureName], frameCount, frameDuration)
    );
    std::cout << "Created animation: " << name << " with texture " << textureName << std::endl;
}

void AssetManager::AddFont(const std::string &name, const std::string &path)
{
    sf::Font font;
    if (font.openFromFile(path))
    {
        _fonts[name] = font;
        std::cout << "Loaded font: " << name << " from " << path << std::endl;
    }
    else
    {
        std::cerr << "Failed to load font: " << name << " from " << path << std::endl;
    }
}

const sf::Texture& AssetManager::GetTexture(const std::string& name) const
{
    if (_textures.count(name) == 0)
        throw std::runtime_error("Texture not found: " + name);
    return *_textures.at(name);  // разыменовываем unique_ptr
}

const Animation& AssetManager::GetAnimation(const std::string &name) const
{
    if (_animations.count(name) == 0)
    {
        throw std::runtime_error("Animation not found: " + name);
    }
    return _animations.at(name);
}

const sf::Font& AssetManager::GetFont(const std::string &name) const
{
    if (_fonts.count(name) == 0)
    {
        throw std::runtime_error("Font not found: " + name);
    }
    return _fonts.at(name);
}


