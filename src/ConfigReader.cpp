#include "ConfigReader.h"
#include <nlohmann/json.hpp>
#include <fstream>
using json = nlohmann::json;
ConfigReader::ConfigReader(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Cannot open config file: " + path);
    file >> _config;
}

unsigned int ConfigReader::GetWindowWidth()         { return _config["window"]["width"]; }
unsigned int ConfigReader::GetWindowHeight()        { return _config["window"]["height"]; }
std::string  ConfigReader::GetFontPath()            { return _config["font"]; }
std::string  ConfigReader::GetLogoPath(int index)   { return _config["logos"][index]; }
int          ConfigReader::GetLogoCount()           { return _config["logos"].size(); }
float        ConfigReader::GetVelocityX()           { return _config["velocity"]["x"]; }
float        ConfigReader::GetVelocityY()           { return _config["velocity"]["y"]; }
float        ConfigReader::GetScale()               { return _config["scale"]; }

sf::Color ConfigReader::GetColor(int index)
{
    // Если colors не задан или для этого индекса нет записи — белый
    if (!_config.contains("colors") || index >= (int)_config["colors"].size())
        return sf::Color::White;

    auto& c = _config["colors"][index];
    if (c.is_null())
        return sf::Color::White;

    return sf::Color(c["r"], c["g"], c["b"],
                     c.contains("a") ? (uint8_t)c["a"] : 255);
}