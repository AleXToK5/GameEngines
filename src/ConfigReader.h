#ifndef DEMO_CONFIGREADER_H
#define DEMO_CONFIGREADER_H

#include <filesystem>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>

class ConfigReader {
    nlohmann::json _config;
    void ValidateConfig();

public:
    explicit ConfigReader(const std::string& path);

    // Window settings
    unsigned int GetWindowWidth() const;
    unsigned int GetWindowHeight() const;

    // Fonts
    std::string GetMainFontPath() const;

    // Player settings
    float GetPlayerMovementSpeed() const;
    int GetPlayerShootCooldownMs() const;

    // Projectile settings
    float GetProjectileSpeed() const;

    // Asteroid settings
    int GetAsteroidSpawnTimeMinMs() const;
    int GetAsteroidSpawnTimeMaxMs() const;
    float GetAsteroidSpeedMin() const;
    float GetAsteroidSpeedMax() const;
    float GetAsteroidSizeMin() const;
    float GetAsteroidSizeMax() const;
    float GetAsteroidDirectionXMin() const;
    float GetAsteroidDirectionXMax() const;
    float GetAsteroidDirectionYMin() const;
    float GetAsteroidDirectionYMax() const;
    int GetAsteroidCirclePointsMin() const;
    int GetAsteroidCirclePointsMax() const;

    // Legacy methods for compatibility
    std::string GetFontPath();

};

#endif //DEMO_CONFIGREADER_H