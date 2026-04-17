#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <SFML/Graphics.hpp>
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../Ecs/Systems/ISystem.h"

#include "../Physics/PositionComponent.h"
#include "RenderComponent.h"
#include "../Core/GameStateComponent.h"

class RenderSystem final : public ISystem {
    ComponentStorage<PositionComponent> &_positionComponents;
    ComponentStorage<RenderComponent> &_renderComponents;
    ComponentStorage<GameStateComponent> &_gameStates;

    sf::RenderWindow &_window;
    sf::Font _font;

    Filter _renderables;
    Filter _gameStateFilter;

public:
    RenderSystem(World &world, sf::RenderWindow &window, const std::string &fontPath)
        : ISystem(world),
          _positionComponents(world.GetStorage<PositionComponent>()),
          _renderComponents(world.GetStorage<RenderComponent>()),
          _gameStates(world.GetStorage<GameStateComponent>()),
          _window(window),
          _renderables(FilterBuilder(world)
              .With<PositionComponent>()
              .With<RenderComponent>()
              .Build()),
          _gameStateFilter(FilterBuilder(world)
              .With<GameStateComponent>()
              .Build()) {
        if (!_font.openFromFile(fontPath)) {
            throw std::runtime_error("Failed to load font: " + fontPath);
        }
    }

    void OnInit() override;

    void OnUpdate() override;
};

#endif //RENDERSYSTEM_H
