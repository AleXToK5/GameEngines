#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"

#include "../Components/PositionComponent.h"
#include "../Components/ColliderComponent.h"
#include "../Components/AsteroidComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/GameStateComponent.h"

#include <unordered_set>

class CollisionSystem final : public ISystem {
    ComponentStorage<PositionComponent>& _positions;
    ComponentStorage<ColliderComponent>& _colliders;
    ComponentStorage<GameStateComponent>& _gameStates;

    Filter _projectilesFilter;
    Filter _asteroidsFilter;
    Filter _playersFilter;
    Filter _gameStateFilter;

public:
    CollisionSystem(World &world);

    void OnInit() override;
    void OnUpdate() override;
};

#endif //COLLISIONSYSTEM_H