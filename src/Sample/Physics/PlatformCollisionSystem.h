#ifndef PLATFORMCOLLISIONSYSTEM_H
#define PLATFORMCOLLISIONSYSTEM_H

#include <cmath>
#include <unordered_set>
#include <vector>
#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "VelocityComponent.h"
#include "../Gameplay/Player/PlayerComponent.h"
#include "../Gameplay/Environment/BrickComponent.h"
#include "../Graphics/DestroyAfterAnimationComponent.h"
#include "../Graphics/AnimatorComponent.h"
#include "../Graphics/SpriteComponent.h"
#include "../Gameplay/Weapons/ProjectileComponent.h"
#include "../Gameplay/Environment/FinishComponent.h"

class PlatformCollisionSystem final : public ISystem {
    World &_world;
    ComponentStorage<TransformComponent> &_transforms;
    ComponentStorage<ColliderComponent> &_colliders;
    ComponentStorage<VelocityComponent> &_velocities;
    ComponentStorage<PlayerComponent> &_players;

    Filter _dynamicFilter;
    Filter _staticFilter;

public:
    PlatformCollisionSystem(World &world)
        : ISystem(world),
          _world(world),
          _transforms(world.GetStorage<TransformComponent>()),
          _colliders(world.GetStorage<ColliderComponent>()),
          _velocities(world.GetStorage<VelocityComponent>()),
          _players(world.GetStorage<PlayerComponent>()),
          _dynamicFilter(
              FilterBuilder(world).With<TransformComponent>().With<ColliderComponent>().With<VelocityComponent>().
              Build()),
          _staticFilter(FilterBuilder(world).With<TransformComponent>().With<ColliderComponent>().Build()) {
    }

    void OnInit() override {
    }

    void OnUpdate() override {
        std::unordered_set<int> entitiesToRemove;
        std::vector<sf::Vector2f> explosionsToCreate;

        for (int dynEnt: _dynamicFilter) {
            auto &dynT = _transforms.Get(dynEnt);
            auto &dynC = _colliders.Get(dynEnt);
            auto &dynV = _velocities.Get(dynEnt);
            bool isPlayer = _players.Has(dynEnt);
            bool isProjectile = _world.GetStorage<ProjectileComponent>().Has(dynEnt);

            dynT.X -= dynV.X;
            dynT.Y -= dynV.Y;
            dynT.X += dynV.X;

            for (int statEnt: _staticFilter) {
                if (dynEnt == statEnt) continue;
                if (_velocities.Has(statEnt)) continue;

                if (isProjectile && _world.GetStorage<FinishComponent>().Has(statEnt)) continue;
                if (isPlayer && _world.GetStorage<FinishComponent>().Has(statEnt)) continue;
                if (entitiesToRemove.contains(statEnt) || entitiesToRemove.contains(dynEnt)) continue;

                auto &statC = _colliders.Get(statEnt);
                auto &statT = _transforms.Get(statEnt);

                float dynHalfW = dynC.Size.x / 2.f;
                float dynHalfH = dynC.Size.y / 2.f;
                float statHalfW = statC.Size.x / 2.f;
                float statHalfH = statC.Size.y / 2.f;

                float dx = dynT.X - statT.X;
                float dy = dynT.Y - statT.Y;
                float intersectX = std::abs(dx) - (dynHalfW + statHalfW);
                float intersectY = std::abs(dy) - (dynHalfH + statHalfH);

                if (intersectX < 0.f && intersectY < 0.f) {
                    if (isProjectile) {
                        if (_world.GetStorage<BrickComponent>().Has(statEnt)) {
                            entitiesToRemove.insert(statEnt);
                            explosionsToCreate.push_back({statT.X, statT.Y});
                        }
                        entitiesToRemove.insert(dynEnt);
                        break;
                    }

                    if (dx > 0) { dynT.X -= intersectX; } else { dynT.X += intersectX; }
                    dynV.X = 0.f;
                }
            }

            if (entitiesToRemove.contains(dynEnt)) continue;
            dynT.Y += dynV.Y;

            for (int statEnt: _staticFilter) {
                if (dynEnt == statEnt || _velocities.Has(statEnt)) continue;
                if (isProjectile && _world.GetStorage<FinishComponent>().Has(statEnt)) continue;
                if (entitiesToRemove.contains(statEnt) || entitiesToRemove.contains(dynEnt)) continue;

                auto &statC = _colliders.Get(statEnt);
                auto &statT = _transforms.Get(statEnt);

                float dx = dynT.X - statT.X;
                float dy = dynT.Y - statT.Y;
                float intersectX = std::abs(dx) - (dynC.Size.x / 2.f + statC.Size.x / 2.f);
                float intersectY = std::abs(dy) - (dynC.Size.y / 2.f + statC.Size.y / 2.f);

                if (intersectX < 0.f && intersectY < 0.f) {
                    if (isProjectile) {
                        if (_world.GetStorage<BrickComponent>().Has(statEnt)) {
                            entitiesToRemove.insert(statEnt);
                            explosionsToCreate.push_back({statT.X, statT.Y});
                        }
                        entitiesToRemove.insert(dynEnt);
                        break;
                    }

                    if (dy > 0) {
                        dynT.Y -= intersectY;
                        dynV.Y = 0.f;

                        if (isPlayer && _world.GetStorage<BrickComponent>().Has(statEnt)) {
                            entitiesToRemove.insert(statEnt);
                            explosionsToCreate.push_back({statT.X, statT.Y});
                        }
                    } else {
                        dynT.Y += intersectY;
                        dynV.Y = 0.f;

                        if (isPlayer) _players.Get(dynEnt).IsGrounded = true;
                    }

                    if (isPlayer && _world.GetStorage<FinishComponent>().Has(statEnt)) {
                        _players.Get(dynEnt).IsFinished = true;
                    }
                }
            }
        }

        for (int e: entitiesToRemove) {
            _world.RemoveEntity(e);
        }

        for (auto pos: explosionsToCreate) {
            int expEnt = _world.CreateEntity();
            _world.GetStorage<TransformComponent>().Add(expEnt, {pos.x, pos.y, 2.0f, 2.0f});
            _world.GetStorage<SpriteComponent>().Add(expEnt, {"ExplosionTex"});
            _world.GetStorage<AnimatorComponent>().Add(expEnt, {"ExplosionAnim", 0, 0});
            _world.GetStorage<DestroyAfterAnimationComponent>().Add(expEnt, {});
        }
    }
};

#endif
