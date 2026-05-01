#ifndef GAMEENGINECONF_H
#define GAMEENGINECONF_H
#include <string>

class GameEngineConfiguration
{
public:
    unsigned int width;
    unsigned int height;
    std::string assetsFile;

    GameEngineConfiguration(unsigned int w = 1280, unsigned int h = 768, const std::string& assets = "assets.json")
        : width(w), height(h), assetsFile(assets) {}
};

#endif //GAMEENGINECONF_H