#ifndef PLATFORMCOLLISIONSYSTEM_H
#define PLATFORMCOLLISIONSYSTEM_H

#include <cmath>
#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "VelocityComponent.h"
#include "../Gameplay/Player/PlayerComponent.h"

class PlatformCollisionSystem final : public ISystem {
    ComponentStorage<TransformComponent> &_transforms;
    ComponentStorage<ColliderComponent> &_colliders;
    ComponentStorage<VelocityComponent> &_velocities;
    ComponentStorage<PlayerComponent> &_players;

    Filter _dynamicFilter;
    Filter _staticFilter;

public:
    PlatformCollisionSystem(World &world)
        : ISystem(world),
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
        for (int dynEnt: _dynamicFilter) {
            auto &dynT = _transforms.Get(dynEnt);
            auto &dynC = _colliders.Get(dynEnt);
            auto &dynV = _velocities.Get(dynEnt);
            bool isPlayer = _players.Has(dynEnt);

            for (int statEnt: _staticFilter) {
                if (dynEnt == statEnt) continue;
                if (_velocities.Has(statEnt)) continue;

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
                    if (intersectX > intersectY) {
                        if (dx > 0) { dynT.X -= intersectX; } else { dynT.X += intersectX; }
                        dynV.X = 0.f;
                    } else {
                        if (dy > 0) {
                            dynT.Y -= intersectY;
                            dynV.Y = 0.f;
                        } else {
                            dynT.Y += intersectY;
                            dynV.Y = 0.f;
                            if (isPlayer) _players.Get(dynEnt).IsGrounded = true;
                        }
                    }
                }
            }
        }
    }
};

#endif
