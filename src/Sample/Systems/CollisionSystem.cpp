#include "CollisionSystem.h"

CollisionSystem::CollisionSystem(World &world)
    : ISystem(world),
      _positions(world.GetStorage<PositionComponent>()),
      _colliders(world.GetStorage<ColliderComponent>()),
      _gameStates(world.GetStorage<GameStateComponent>()),
      _projectilesFilter(
          FilterBuilder(world).With<PositionComponent>().With<ColliderComponent>().With<ProjectileComponent>().Build()),
      _asteroidsFilter(
          FilterBuilder(world).With<PositionComponent>().With<ColliderComponent>().With<AsteroidComponent>().Build()),
      _playersFilter(
          FilterBuilder(world).With<PositionComponent>().With<ColliderComponent>().With<PlayerComponent>().Build()),
      _gameStateFilter(FilterBuilder(world).With<GameStateComponent>().Build()) {
}

void CollisionSystem::OnInit() {
}

void CollisionSystem::OnUpdate() {
    GameStateComponent *gameState = nullptr;
    for (int ent: _gameStateFilter) {
        gameState = &_gameStates.Get(ent);
        break;
    }

    std::unordered_set<int> entitiesToRemove;

    for (const int proj: _projectilesFilter) {
        if (entitiesToRemove.count(proj)) continue;
        auto &projPos = _positions.Get(proj);
        auto &projCol = _colliders.Get(proj);

        for (const int ast: _asteroidsFilter) {
            if (entitiesToRemove.count(ast)) continue;
            auto &astPos = _positions.Get(ast);
            auto &astCol = _colliders.Get(ast);

            float halfW = projCol.Size.x / 2.0f;
            float halfH = projCol.Size.y / 2.0f;

            // Находим границы прямоугольника (пули)
            float minX = projPos.X - halfW;
            float maxX = projPos.X + halfW;
            float minY = projPos.Y - halfH;
            float maxY = projPos.Y + halfH;

            // Ищем ближайшую точку на прямоугольнике к центру круга (астероида)
            float closestX = std::max(minX, std::min(astPos.X, maxX));
            float closestY = std::max(minY, std::min(astPos.Y, maxY));

            // Расстояние от ближайшей точки до центра круга
            float dx = astPos.X - closestX;
            float dy = astPos.Y - closestY;

            // Если квадрат расстояния меньше квадрата радиуса — столкновение
            if ((dx * dx + dy * dy) < (astCol.Radius * astCol.Radius)) {
                entitiesToRemove.insert(proj);
                entitiesToRemove.insert(ast);
                if (gameState) gameState->Score++;
                break;
            }
        }
    }

    if (gameState && !gameState->IsGameOver) {
        for (const int player: _playersFilter) {
            if (entitiesToRemove.count(player)) continue;
            auto &playerPos = _positions.Get(player);
            auto &playerCol = _colliders.Get(player);

            for (const int ast: _asteroidsFilter) {
                if (entitiesToRemove.count(ast)) continue;
                auto &astPos = _positions.Get(ast);
                auto &astCol = _colliders.Get(ast);

                float dx = playerPos.X - astPos.X;
                float dy = playerPos.Y - astPos.Y;
                float distanceSq = (dx * dx) + (dy * dy);
                float radiusSumSq = (playerCol.Radius + astCol.Radius) * (playerCol.Radius + astCol.Radius);

                if (distanceSq < radiusSumSq) {
                    entitiesToRemove.insert(player);
                    gameState->IsGameOver = true;
                    break;
                }
            }
        }
    }

    for (int ent: entitiesToRemove) {
        world.RemoveEntity(ent);
    }
}
