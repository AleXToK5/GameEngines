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
#include "../Physics/VelocityComponent.h"
#include "../Physics/GravityComponent.h"
#include "../Physics/ColliderComponent.h"
#include "../Graphics/SpriteComponent.h"
#include "../Gameplay/Player/PlayerComponent.h"
#include "../Gameplay/PlayerTag.h"
#include "../Gameplay/Environment/BrickComponent.h"
#include "../Gameplay/Environment/FinishComponent.h"
#include "../Gameplay/Environment/DecorComponent.h"

static const std::unordered_map<std::string, std::string> LevelObjectTextures = {
    {"Player", "MegaIdle"},
    {"Tile", "Tile"},
    {"Brick", "Brick Tile"},
    {"BigHill", "BigHill"},
    {"Finish", "Finish"},
    {"Cloud", "Cloud"},
};

class LevelInitializer final : public IInitializer {
    std::string _levelFile;
    std::string _configFile;
    const AssetManager &_assets;
    float _windowHeight;

    static constexpr float TileSize = 64.f;

public:
    LevelInitializer(World &world, const std::string &levelFile, const std::string &configFile,
                     const AssetManager &assets, float windowHeight)
        : IInitializer(world), _levelFile(levelFile), _configFile(configFile), _assets(assets),
          _windowHeight(windowHeight) {
    }

    void OnInit() override {
        std::ifstream confFile(_configFile);
        nlohmann::json configData;
        if (confFile.is_open()) { confFile >> configData; }

        std::ifstream file(_levelFile);
        if (!file.is_open()) return;

        nlohmann::json data;
        try { file >> data; } catch (...) { return; }

        for (const auto &obj: data) {
            std::string name = obj.value("name", "");
            float gridX = obj.value("x", 0);
            float gridY = obj.value("y", 0);

            float px = gridX * TileSize + TileSize / 2.f;
            float py = _windowHeight - (gridY * TileSize) - TileSize / 2.f;

            auto it = LevelObjectTextures.find(name);
            if (it == LevelObjectTextures.end() && name != "Player") continue;

            int e = world.CreateEntity();
            world.GetStorage<TransformComponent>().Add(e, {px, py, 1.f, 1.f});

            if (name == "Player") {
                world.GetStorage<PlayerTag>().Add(e, {});
                world.GetStorage<AnimatorComponent>().Add(e, {"IdleAnim", 0, 0});
                world.GetStorage<SpriteComponent>().Add(e, {"MegaIdle"});

                if (configData.contains("Player")) {
                    auto &pData = configData["Player"];
                    float bboxW = pData["Bbox"][0];
                    float bboxH = pData["Bbox"][1];
                    float scale = pData.value("Scale", 2.0f);

                    auto &t = world.GetStorage<TransformComponent>().Get(e);
                    t.ScaleX = scale;
                    t.ScaleY = scale;

                    world.GetStorage<PlayerComponent>().Add(e, {
                                                                pData["SX"], pData["JY"], pData["SM"], false, 0, 20,
                                                                false, px, py
                                                            });
                    world.GetStorage<VelocityComponent>().Add(e, {0.f, 0.f});
                    world.GetStorage<GravityComponent>().Add(e, {pData["GY"], pData["SM"]});
                    world.GetStorage<ColliderComponent>().Add(e, {
                                                                  ColliderType::AABB, 0.f, {bboxW, bboxH}, Player,
                                                                  static_cast<uint16_t>(Asteroid)
                                                              });
                }
            } else if (name == "Tile" || name == "Brick" || name == "Finish") {
                world.GetStorage<SpriteComponent>().Add(e, {it->second});

                if (name == "Finish") {
                    float finishHeight = TileSize * 8.0f;
                    auto &t = world.GetStorage<TransformComponent>().Get(e);
                    t.Y -= (TileSize * 3.5f);

                    world.GetStorage<ColliderComponent>().Add(e, {
                                                                  ColliderType::AABB, 0.f, {TileSize, finishHeight},
                                                                  Asteroid, static_cast<uint16_t>(Player)
                                                              });
                    world.GetStorage<FinishComponent>().Add(e, {});
                } else {
                    world.GetStorage<ColliderComponent>().Add(e, {
                                                                  ColliderType::AABB, 0.f, {TileSize, TileSize},
                                                                  Asteroid, static_cast<uint16_t>(Player | Projectile)
                                                              });

                    if (name == "Brick") world.GetStorage<BrickComponent>().Add(e, {});
                }
            } else if (name == "BigHill" || name == "Cloud") {
                world.GetStorage<SpriteComponent>().Add(e, {it->second});
                world.GetStorage<DecorComponent>().Add(e, {});
            }
        }
    }
};

#endif //LEVELINITIALIZER_H
