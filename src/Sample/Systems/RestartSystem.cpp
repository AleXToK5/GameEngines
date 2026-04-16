#include "RestartSystem.h"
#include <vector>

RestartSystem::RestartSystem(World &world, ConfigReader &config)
    : ISystem(world),
      _config(config),
      _gameStates(world.GetStorage<GameStateComponent>()),
      _positions(world.GetStorage<PositionComponent>()),
      _movements(world.GetStorage<MovementComponent>()),
      _renders(world.GetStorage<RenderComponent>()),
      _colliders(world.GetStorage<ColliderComponent>()),
      _players(world.GetStorage<PlayerComponent>()),
      _gameStateFilter(FilterBuilder(world).With<GameStateComponent>().Build()),
      _asteroidsFilter(FilterBuilder(world).With<AsteroidComponent>().Build()),
      _projectilesFilter(FilterBuilder(world).With<ProjectileComponent>().Build()),
      _playersFilter(FilterBuilder(world).With<PlayerComponent>().Build()) {
}

void RestartSystem::OnUpdate() {
    // Ищем компонент состояния игры
    GameStateComponent *gameState = nullptr;
    for (int ent: _gameStateFilter) {
        gameState = &_gameStates.Get(ent);
        break;
    }

    if (!gameState || !gameState->IsGameOver) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
        gameState->Score = 0;
        gameState->IsGameOver = false;

        std::vector<int> entitiesToRemove;
        for (int ast: _asteroidsFilter) entitiesToRemove.push_back(ast);
        for (int proj: _projectilesFilter) entitiesToRemove.push_back(proj);
        for (int pl: _playersFilter) entitiesToRemove.push_back(pl);

        for (int ent: entitiesToRemove) {
            world.RemoveEntity(ent);
        }

        const int player = world.CreateEntity();

        float startX = _config.GetWindowWidth() / 2.0f;
        float startY = _config.GetWindowHeight() - 50.0f;

        _positions.Add(player, PositionComponent{startX, startY});
        _movements.Add(player, MovementComponent{_config.GetPlayerMovementSpeed() / 60.0f, sf::Vector2f(0.f, 0.f)});
        _renders.Add(player, RenderComponent{ShapeType::Polygon, 20.f, 3, {0.f, 0.f}, sf::Color::Green});
        _colliders.Add(player, ColliderComponent{ColliderType::Circle, 20.f, {0.f, 0.f}});

        int cooldown = (_config.GetPlayerShootCooldownMs() * 60) / 1000;
        _players.Add(player, PlayerComponent{0, cooldown});
    }
}
