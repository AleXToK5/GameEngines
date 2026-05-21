#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace EditorConstants {

    constexpr float TileSize = 64.f;
    constexpr float ZoomMin  = 0.25f;
    constexpr float ZoomMax  = 4.f;
    constexpr float ZoomStep = 0.1f;

    inline const std::vector<std::string> PlaceableObjects = {
        "Player", "Tile", "Brick", "BigHill", "Cloud", "Finish"
    };

    inline const std::unordered_map<std::string, std::string> ObjectTextures = {
        {"Player",  "MegaIdle"},
        {"Tile",    "Tile"},
        {"Brick",   "Brick Tile"},
        {"BigHill", "BigHill"},
        {"Cloud",   "Cloud"},
        {"Finish",  "Finish"},
    };

    inline std::string TextureToObjectName(const std::string& texName) {
        for (const auto& [obj, tex] : ObjectTextures) {
            if (tex == texName) return obj;
        }
        return "";
    }

}