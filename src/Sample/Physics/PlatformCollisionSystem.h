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
#include "../Gameplay/Environment/GoombaComponent.h"

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
                if (dynEnt == statEnt || _velocities.Has(statEnt)) continue;
                if (isProjectile && _world.GetStorage<FinishComponent>().Has(statEnt)) continue;
                if (isPlayer && _world.GetStorage<FinishComponent>().Has(statEnt)) continue;
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

        std::vector<int> dynamics;
        for (int e: _dynamicFilter) {
            dynamics.push_back(e);
        }

        for (size_t i = 0; i < dynamics.size(); ++i) {
            int e1 = dynamics[i];
            if (entitiesToRemove.contains(e1)) continue;

            for (size_t j = i + 1; j < dynamics.size(); ++j) {
                int e2 = dynamics[j];
                if (entitiesToRemove.contains(e2)) continue;

                auto &c1 = _colliders.Get(e1);
                auto &c2 = _colliders.Get(e2);
                auto &t1 = _transforms.Get(e1);
                auto &t2 = _transforms.Get(e2);

                float dx = t1.X - t2.X;
                float dy = t1.Y - t2.Y;
                float intersectX = std::abs(dx) - (c1.Size.x / 2.f + c2.Size.x / 2.f);
                float intersectY = std::abs(dy) - (c1.Size.y / 2.f + c2.Size.y / 2.f);

                if (intersectX < 0.f && intersectY < 0.f) {
                    bool e1IsPlayer = _players.Has(e1);
                    bool e2IsPlayer = _players.Has(e2);
                    bool e1IsGoomba = _world.GetStorage<GoombaComponent>().Has(e1);
                    bool e2IsGoomba = _world.GetStorage<GoombaComponent>().Has(e2);
                    bool e1IsProj = _world.GetStorage<ProjectileComponent>().Has(e1);
                    bool e2IsProj = _world.GetStorage<ProjectileComponent>().Has(e2);

                    if ((e1IsPlayer && e2IsGoomba) || (e2IsPlayer && e1IsGoomba)) {
                        int playerEnt = e1IsPlayer ? e1 : e2;
                        auto &t = _transforms.Get(playerEnt);
                        auto &p = _players.Get(playerEnt);
                        auto &v = _velocities.Get(playerEnt);

                        t.X = p.SpawnX;
                        t.Y = p.SpawnY;
                        v.X = 0;
                        v.Y = 0;
                    } else if ((e1IsProj && e2IsGoomba) || (e2IsProj && e1IsGoomba)) {
                        int projEnt = e1IsProj ? e1 : e2;
                        int goombaEnt = e1IsGoomba ? e1 : e2;
                        entitiesToRemove.insert(projEnt);
                        entitiesToRemove.insert(goombaEnt);

                        explosionsToCreate.push_back({_transforms.Get(goombaEnt).X, _transforms.Get(goombaEnt).Y});
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
