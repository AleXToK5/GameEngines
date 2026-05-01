#ifndef LEVELINITIALIZER_H
#define LEVELINITIALIZER_H

#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include "../../Ecs/Systems/IInitializer.h"
#include "../../GameEngine/Assets/AssetManager.h"
#include "../Physics/TransformComponent.h"
#include "../Graphics/SpriteComponent.h"
#include "../Gameplay/PlayerTag.h"

// Маппинг имён объектов уровня -> имена текстур в AssetManager
static const std::unordered_map<std::string, std::string> LevelObjectTextures = {
    { "Player",   "MegaIdle"   },
    { "Tile",     "Tile"       },
    { "Brick",    "Brick Tile" },
    { "BigHill",  "BigHill"    },
    { "Pipe",     "Pipe Up R"  },
    { "Finish",   "Finish"     },
};

class LevelInitializer final : public IInitializer
{
    std::string _levelFile;
    const AssetManager& _assets;

    static constexpr float TileSize = 64.f;

public:
    LevelInitializer(World& world, const std::string& levelFile, const AssetManager& assets)
        : IInitializer(world), _levelFile(levelFile), _assets(assets)
    {}

    void OnInit() override
    {
        std::ifstream file(_levelFile);
        if (!file.is_open())
        {
            std::cerr << "LevelInitializer: cannot open " << _levelFile << std::endl;
            return;
        }

        nlohmann::json data;
        try { file >> data; }
        catch (const std::exception& e)
        {
            std::cerr << "LevelInitializer: JSON parse error: " << e.what() << std::endl;
            return;
        }

        for (const auto& obj : data)
        {
            std::string name = obj.value("name", "");
            float gridX = obj.value("x", 0);
            float gridY = obj.value("y", 0);

            // Перевод координат сетки в пиксели
            float px = gridX * TileSize;
            float py = gridY * TileSize;

            auto it = LevelObjectTextures.find(name);
            if (it == LevelObjectTextures.end())
            {
                std::cerr << "LevelInitializer: unknown object '" << name << "'" << std::endl;
                continue;
            }

            const std::string& texName = it->second;

            // Проверяем что текстура есть
            try { _assets.GetTexture(texName); }
            catch (...)
            {
                std::cerr << "LevelInitializer: texture not found '" << texName << "'" << std::endl;
                continue;
            }

            int e = world.CreateEntity();
            world.GetStorage<TransformComponent>().Add(e, {px, py, 1.f, 1.f});
            world.GetStorage<SpriteComponent>().Add(e, {texName});

            if (name == "Player")
                world.GetStorage<PlayerTag>().Add(e, {});
        }
    }
};

#endif //LEVELINITIALIZER_H