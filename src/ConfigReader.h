#ifndef DEMO_CONFIGREADER_H
#define DEMO_CONFIGREADER_H
#include <filesystem>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>

class ConfigReader {
    nlohmann::json _config;
public:

    ConfigReader(const std::string& path);

    unsigned int    GetWindowWidth();
    unsigned int    GetWindowHeight();

    std::string     GetFontPath();
    std::string     GetLogoPath(int index);
    int             GetLogoCount();

    float           GetVelocityX();
    float           GetVelocityY();
    float           GetScale();
    sf::Color       GetColor(int index);
};


#endif //DEMO_CONFIGREADER_H