#include "Text.h"

Text::Text(const Text& other)       { Transfer(other); }
Text::Text(Text&& other)            { Transfer(other); }

Text::Text(const std::filesystem::path& fontPath, const sf::String& text, int fontSize)
{
    if (!_font.openFromFile(fontPath)) {
        std::cerr << "Could not load font!\n";
        exit(-1);
    }
    _text = sf::Text(_font, text, fontSize);
}

void Text::Draw(sf::RenderTarget& target) { target.draw(_text); }

int  Text::GetCharacterSize() const { return _text.getCharacterSize(); }

void Text::SetPosition(sf::Vector2f position) { _text.setPosition(position); }

void Text::CenterIn(const sf::Vector2u& windowSize)
{
    auto bounds = _text.getLocalBounds();
    _text.setPosition({
        (windowSize.x - bounds.size.x) / 2.f,
        (windowSize.y - bounds.size.y) / 2.f
    });
}

Text& Text::operator=(const Text& other) { Transfer(other); return *this; }
Text& Text::operator=(Text&& other)      { Transfer(other); return *this; }

void Text::Transfer(const Text& other)
{
    _font = other._font;
    _text = other._text;
    _text.setFont(_font);
}