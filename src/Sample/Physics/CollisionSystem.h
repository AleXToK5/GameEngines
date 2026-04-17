#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"

#include "PositionComponent.h"
#include "ColliderComponent.h"
#include "../Gameplay/Enemies/AsteroidComponent.h"
#include "../Gameplay/Weapons/ProjectileComponent.h"
#include "../Gameplay/Player/PlayerComponent.h"
#include "../Core/GameStateComponent.h"

#include <unordered_set>

class CollisionSystem final : public ISystem {
    ComponentStorage<PositionComponent> &_positions;
    ComponentStorage<ColliderComponent> &_colliders;
    ComponentStorage<GameStateComponent> &_gameStates;

    ComponentStorage<PlayerComponent> &_players;
    ComponentStorage<ProjectileComponent> &_projectiles;
    ComponentStorage<AsteroidComponent> &_asteroids;

    Filter _collidableFilter;
    Filter _gameStateFilter;

    bool CheckIntersection(const PositionComponent &p1, const ColliderComponent &c1,
                           const PositionComponent &p2, const ColliderComponent &c2);

public:
    CollisionSystem(World &world);

    void OnInit() override {
    }

    void OnUpdate() override;
};

#endif //COLLISIONSYSTEM_H
