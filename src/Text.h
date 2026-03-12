#ifndef TEXT_H
#define TEXT_H

#include <filesystem>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Object.h"

class Text : public Object
{
    sf::Font _font;
    sf::Text _text = sf::Text(_font);

    void Transfer(const Text& other);
public:
    Text() = default;
    Text(const Text& other);
    Text(Text&& other);
    Text(const std::filesystem::path& fontPath, const sf::String& text, int fontSize);

    int  GetCharacterSize() const;
    void SetPosition(sf::Vector2f position);
    void SetString(const sf::String& str) { _text.setString(str); }
    void CenterIn(const sf::Vector2u& windowSize);  // центрирует текст в окне

    void Update(float deltaTime, const sf::Vector2u& windowSize) override {}
    void Draw(sf::RenderTarget& target) override;

    Text& operator=(const Text& other);
    Text& operator=(Text&& other);
};

#endif //TEXT_H