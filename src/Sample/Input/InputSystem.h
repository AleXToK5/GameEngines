#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"

#include "../Gameplay/Player/PlayerComponent.h"
#include "../Physics/MovementComponent.h"

class InputSystem final : public ISystem {
    sf::RenderWindow &_window;

    ComponentStorage<PlayerComponent> &_players;
    ComponentStorage<MovementComponent> &_movements;
    Filter _playerFilter;

public:
    InputSystem(World &world, sf::RenderWindow &window)
        : ISystem(world),
          _window(window),
          _players(world.GetStorage<PlayerComponent>()),
          _movements(world.GetStorage<MovementComponent>()),
          _playerFilter(FilterBuilder(world).With<PlayerComponent>().With<MovementComponent>().Build()) {
    }

    void OnInit() override {
    }

    void OnUpdate() override;
};

#endif //INPUTSYSTEM_H
