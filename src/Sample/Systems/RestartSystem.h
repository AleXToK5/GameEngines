#ifndef RESTARTSYSTEM_H
#define RESTARTSYSTEM_H

#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../ConfigReader.h"
#include <SFML/Window/Keyboard.hpp>

#include "../Components/GameStateComponent.h"
#include "../Components/AsteroidComponent.h"
#include "../Components/ProjectileComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/RenderComponent.h"
#include "../Components/ColliderComponent.h"

class RestartSystem final : public ISystem {
    ConfigReader& _config;

    ComponentStorage<GameStateComponent>& _gameStates;
    ComponentStorage<PositionComponent>& _positions;
    ComponentStorage<MovementComponent>& _movements;
    ComponentStorage<RenderComponent>& _renders;
    ComponentStorage<ColliderComponent>& _colliders;
    ComponentStorage<PlayerComponent>& _players;

    Filter _gameStateFilter;
    Filter _asteroidsFilter;
    Filter _projectilesFilter;
    Filter _playersFilter;

public:
    RestartSystem(World &world, ConfigReader& config);

    void OnInit() override {}
    void OnUpdate() override;
};

#endif //RESTARTSYSTEM_H