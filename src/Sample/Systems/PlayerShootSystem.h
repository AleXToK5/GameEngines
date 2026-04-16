#ifndef PLAYERSHOOTSYSTEM_H
#define PLAYERSHOOTSYSTEM_H

#include <SFML/Window/Keyboard.hpp>
#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../ConfigReader.h"

#include "../Components/PlayerComponent.h"
#include "../Components/PositionComponent.h"
#include "../Components/MovementComponent.h"
#include "../Components/RenderComponent.h"
#include "../Components/ColliderComponent.h"
#include "../Components/ProjectileComponent.h"

class PlayerShootSystem final : public ISystem {
    ConfigReader& _config;
    ComponentStorage<PlayerComponent>& _players;
    ComponentStorage<PositionComponent>& _positions;
    ComponentStorage<MovementComponent>& _movements;
    ComponentStorage<RenderComponent>& _renders;
    ComponentStorage<ColliderComponent>& _colliders;
    ComponentStorage<ProjectileComponent>& _projectiles;

    Filter _playerFilter;

public:
    PlayerShootSystem(World &world, ConfigReader& config);

    void OnInit() override;
    void OnUpdate() override;
};

#endif //PLAYERSHOOTSYSTEM_H