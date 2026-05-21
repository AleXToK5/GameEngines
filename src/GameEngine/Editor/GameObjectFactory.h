#pragma once
#include <string>
#include "Constants.h"
#include "../../Ecs/World/World.h"
#include "../../Sample/Physics/TransformComponent.h"
#include "../../Sample/Physics/ColliderComponent.h"
#include "../../Sample/Graphics/SpriteComponent.h"
#include "../../Sample/Gameplay/Environment/BrickComponent.h"
#include "../../Sample/Gameplay/Environment/FinishComponent.h"
#include "../../Sample/Gameplay/Environment/DecorComponent.h"
#include "../../Sample/Gameplay/Player/PlayerComponent.h"
#include "../../Sample/Gameplay/PlayerTag.h"
#include "../../Sample/Graphics/AnimatorComponent.h"
#include "../../Sample/Gameplay/Environment/GoombaComponent.h"

class GameObjectFactory {
public:
    static int Create(World &world, const std::string &name, float worldX, float worldY) {
        auto it = EditorConstants::ObjectTextures.find(name);
        if (it == EditorConstants::ObjectTextures.end() && name != "Player" && name != "Goomba") return -1;

        int e = world.CreateEntity();
        world.GetStorage<TransformComponent>().Add(e, {worldX, worldY, 1.f, 1.f});

        if (name == "Player") {
            world.GetStorage<SpriteComponent>().Add(e, {"MegaIdle"});
            world.GetStorage<AnimatorComponent>().Add(e, {"IdleAnim", 0, 0});
            world.GetStorage<PlayerTag>().Add(e, {});
            world.GetStorage<PlayerComponent>().Add(e, {
                                                        5.f, -15.f, 12.f, false, 0, 20, false, worldX, worldY
                                                    });
            auto &t = world.GetStorage<TransformComponent>().Get(e);
            t.ScaleX = 2.f;
            t.ScaleY = 2.f;
            world.GetStorage<ColliderComponent>().Add(e, {
                                                          ColliderType::AABB, 0.f, {48.f, 64.f},
                                                          Player,
                                                          static_cast<uint16_t>(Tile)
                                                      });
        } else if (name == "Goomba") {
            world.GetStorage<SpriteComponent>().Add(e, {"GoombaMove"});
            world.GetStorage<AnimatorComponent>().Add(e, {"GoombaWalkAnim", 0, 0});

            auto &t = world.GetStorage<TransformComponent>().Get(e);
            t.ScaleX = 2.f;
            t.ScaleY = 2.f;

            world.GetStorage<GoombaComponent>().Add(e, {
                                                        GoombaState::Patrol,
                                                        worldX - 128.f,
                                                        worldX + 128.f,
                                                        2.0f,
                                                        -1
                                                    });

            world.GetStorage<VelocityComponent>().Add(e, {0.f, 0.f});
            world.GetStorage<GravityComponent>().Add(e, {0.8f, 20.f});

            world.GetStorage<ColliderComponent>().Add(e, {
                                                          ColliderType::AABB, 0.f, {48.f, 48.f},
                                                          Enemy, static_cast<uint16_t>(Tile | Player | Projectile)
                                                      });
        } else if (name == "Tile") {
            world.GetStorage<SpriteComponent>().Add(e, {"Tile"});
            world.GetStorage<ColliderComponent>().Add(e, {
                                                          ColliderType::AABB, 0.f,
                                                          {EditorConstants::TileSize, EditorConstants::TileSize},
                                                          Tile, static_cast<uint16_t>(Player | Projectile)
                                                      });
        } else if (name == "Brick") {
            world.GetStorage<SpriteComponent>().Add(e, {"Brick Tile"});
            world.GetStorage<ColliderComponent>().Add(e, {
                                                          ColliderType::AABB, 0.f,
                                                          {EditorConstants::TileSize, EditorConstants::TileSize},
                                                          Tile, static_cast<uint16_t>(Player | Projectile)
                                                      });
            world.GetStorage<BrickComponent>().Add(e, {});
        } else if (name == "Finish") {
            world.GetStorage<SpriteComponent>().Add(e, {"Finish"});
            float finishHeight = EditorConstants::TileSize * 8.f;
            world.GetStorage<ColliderComponent>().Add(e, {
                                                          ColliderType::AABB, 0.f,
                                                          {EditorConstants::TileSize, finishHeight},
                                                          Tile, static_cast<uint16_t>(Player)
                                                      });
            world.GetStorage<FinishComponent>().Add(e, {});
        } else if (name == "BigHill" || name == "Cloud") {
            world.GetStorage<SpriteComponent>().Add(e, {it->second});
            world.GetStorage<DecorComponent>().Add(e, {});
        }

        return e;
    }
};
