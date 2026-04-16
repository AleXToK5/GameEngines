#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../ConfigReader.h"

#include "../Components/PositionComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/ColliderComponent.h"

class MovementSystem final : public ISystem {
    ConfigReader &_config;
    ComponentStorage<PositionComponent> &_positions;
    ComponentStorage<MovementComponent> &_movements;
    ComponentStorage<PlayerComponent> &_players;
    ComponentStorage<ColliderComponent> &_colliders;

    Filter _moveables;

public:
    MovementSystem(World &world, ConfigReader &config);

    void OnInit() override {
    }

    void OnUpdate() override;
};

#endif //MOVEMENTSYSTEM_H
