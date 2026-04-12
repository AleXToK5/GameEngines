#include "ConfigReader.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

ConfigReader::ConfigReader(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Cannot open config file: " + path);
    file >> _config;
    ValidateConfig();
}

void ConfigReader::ValidateConfig()
{
    // Проверка обязательных полей
    if (!_config.contains("window"))
        throw std::runtime_error("Config missing 'window' section");
    if (!_config["window"].contains("width") || !_config["window"].contains("height"))
        throw std::runtime_error("Config 'window' missing 'width' or 'height'");

    if (!_config.contains("fonts") || !_config["fonts"].contains("main"))
        throw std::runtime_error("Config missing 'fonts.main'");

    if (!_config.contains("player"))
        throw std::runtime_error("Config missing 'player' section");

    if (!_config.contains("projectile"))
        throw std::runtime_error("Config missing 'projectile' section");

    if (!_config.contains("asteroid"))
        throw std::runtime_error("Config missing 'asteroid' section");
}

// Window settings
unsigned int ConfigReader::GetWindowWidth() const
{
    return _config["window"]["width"];
}

unsigned int ConfigReader::GetWindowHeight() const
{
    return _config["window"]["height"];
}

// Fonts
std::string ConfigReader::GetMainFontPath() const
{
    return _config["fonts"]["main"];
}

// Player settings
float ConfigReader::GetPlayerMovementSpeed() const
{
    return _config["player"]["movement_speed"];
}

int ConfigReader::GetPlayerShootCooldownMs() const
{
    return _config["player"]["shoot_cooldown_ms"];
}

// Projectile settings
float ConfigReader::GetProjectileSpeed() const
{
    return _config["projectile"]["speed"];
}

// Asteroid settings
int ConfigReader::GetAsteroidSpawnTimeMinMs() const
{
    return _config["asteroid"]["spawn_time_min_ms"];
}

int ConfigReader::GetAsteroidSpawnTimeMaxMs() const
{
    return _config["asteroid"]["spawn_time_max_ms"];
}

float ConfigReader::GetAsteroidSpeedMin() const
{
    return _config["asteroid"]["speed_min"];
}

float ConfigReader::GetAsteroidSpeedMax() const
{
    return _config["asteroid"]["speed_max"];
}

float ConfigReader::GetAsteroidSizeMin() const
{
    return _config["asteroid"]["size_min"];
}

float ConfigReader::GetAsteroidSizeMax() const
{
    return _config["asteroid"]["size_max"];
}

float ConfigReader::GetAsteroidDirectionXMin() const
{
    return _config["asteroid"]["direction_x_min"];
}

float ConfigReader::GetAsteroidDirectionXMax() const
{
    return _config["asteroid"]["direction_x_max"];
}

float ConfigReader::GetAsteroidDirectionYMin() const
{
    return _config["asteroid"]["direction_y_min"];
}

float ConfigReader::GetAsteroidDirectionYMax() const
{
    return _config["asteroid"]["direction_y_max"];
}

int ConfigReader::GetAsteroidCirclePointsMin() const
{
    return _config["asteroid"]["circle_points_min"];
}

int ConfigReader::GetAsteroidCirclePointsMax() const
{
    return _config["asteroid"]["circle_points_max"];
}

// Legacy methods for backward compatibility
std::string ConfigReader::GetFontPath()
{
    return GetMainFontPath();
}
