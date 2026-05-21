#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

#include "../../Ecs/World/World.h"
#include "../../Sample/Physics/TransformComponent.h"
#include "../../Sample/Graphics/SpriteComponent.h"
#include "Constants.h"

using json = nlohmann::json;

class JsonSerializer {
public:
    static void SaveLevel(const std::string& path, World& world, float windowHeight) {
        auto& transforms = world.GetStorage<TransformComponent>();
        auto& sprites    = world.GetStorage<SpriteComponent>();

        json arr = json::array();

        for (int id = 0; id < world.EntityCount(); id++) {
            if (!world.IsEntityAlive(id)) continue;
            if (!transforms.Has(id) || !sprites.Has(id)) continue;

            const auto& t = transforms.Get(id);
            const auto& s = sprites.Get(id);

            std::string objName = EditorConstants::TextureToObjectName(s.TextureName);
            if (objName.empty()) continue;

            int tileX = static_cast<int>((t.X - EditorConstants::TileSize / 2.f) / EditorConstants::TileSize);
            int tileY = static_cast<int>((windowHeight - t.Y - EditorConstants::TileSize / 2.f) / EditorConstants::TileSize);

            arr.push_back({{"name", objName}, {"x", tileX}, {"y", tileY}});
        }

        std::ofstream file(path);
        if (!file.is_open()) {
            std::cerr << "[JsonSerializer] Cannot open: " << path << std::endl;
            return;
        }
        file << arr.dump(2);
        std::cout << "[JsonSerializer] Saved " << arr.size() << " objects to " << path << std::endl;
    }
};