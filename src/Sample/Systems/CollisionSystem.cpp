#include "CollisionSystem.h"
#include <vector>

CollisionSystem::CollisionSystem(World &world)
    : ISystem(world),
      _positions(world.GetStorage<PositionComponent>()),
      _colliders(world.GetStorage<ColliderComponent>()),
      _gameStates(world.GetStorage<GameStateComponent>()),
      _players(world.GetStorage<PlayerComponent>()),
      _projectiles(world.GetStorage<ProjectileComponent>()),
      _asteroids(world.GetStorage<AsteroidComponent>()),
      _collidableFilter(FilterBuilder(world).With<PositionComponent>().With<ColliderComponent>().Build()),
      _gameStateFilter(FilterBuilder(world).With<GameStateComponent>().Build()) {
}

bool CollisionSystem::CheckIntersection(const PositionComponent &p1, const ColliderComponent &c1,
                                        const PositionComponent &p2, const ColliderComponent &c2) {
    // Circle - Circle
    if (c1.Type == ColliderType::Circle && c2.Type == ColliderType::Circle) {
        float dx = p1.X - p2.X;
        float dy = p1.Y - p2.Y;
        float distSq = dx * dx + dy * dy;
        float radSum = c1.Radius + c2.Radius;
        return distSq < (radSum * radSum);
    }

    // Сircle - AABB
    const PositionComponent *circPos = (c1.Type == ColliderType::Circle) ? &p1 : &p2;
    const ColliderComponent *circCol = (c1.Type == ColliderType::Circle) ? &c1 : &c2;
    const PositionComponent *aabbPos = (c1.Type == ColliderType::AABB) ? &p1 : &p2;
    const ColliderComponent *aabbCol = (c1.Type == ColliderType::AABB) ? &c1 : &c2;

    float halfW = aabbCol->Size.x / 2.0f;
    float halfH = aabbCol->Size.y / 2.0f;

    float minX = aabbPos->X - halfW;
    float maxX = aabbPos->X + halfW;
    float minY = aabbPos->Y - halfH;
    float maxY = aabbPos->Y + halfH;

    float closestX = std::max(minX, std::min(circPos->X, maxX));
    float closestY = std::max(minY, std::min(circPos->Y, maxY));

    float dx = circPos->X - closestX;
    float dy = circPos->Y - closestY;

    return (dx * dx + dy * dy) < (circCol->Radius * circCol->Radius);
}

void CollisionSystem::OnUpdate() {
    GameStateComponent *gameState = nullptr;
    for (int ent: _gameStateFilter) {
        gameState = &_gameStates.Get(ent);
        break;
    }

    if (gameState && gameState->IsGameOver) return;

    std::vector<int> collidables;
    for (int ent: _collidableFilter) {
        collidables.push_back(ent);
    }

    std::unordered_set<int> entitiesToRemove;

    for (size_t i = 0; i < collidables.size(); ++i) {
        for (size_t j = i + 1; j < collidables.size(); ++j) {
            int e1 = collidables[i];
            int e2 = collidables[j];

            if (entitiesToRemove.count(e1) || entitiesToRemove.count(e2)) continue;

            auto &c1 = _colliders.Get(e1);
            auto &c2 = _colliders.Get(e2);

            if (!(c1.Layer & c2.Mask) || !(c2.Layer & c1.Mask)) {
                continue;
            }

            auto &p1 = _positions.Get(e1);
            auto &p2 = _positions.Get(e2);

            if (CheckIntersection(p1, c1, p2, c2)) {
                bool isE1Player = _players.Has(e1);
                bool isE2Player = _players.Has(e2);
                bool isE1Ast = _asteroids.Has(e1);
                bool isE2Ast = _asteroids.Has(e2);
                bool isE1Proj = _projectiles.Has(e1);
                bool isE2Proj = _projectiles.Has(e2);

                if ((isE1Player && isE2Ast) || (isE2Player && isE1Ast)) {
                    entitiesToRemove.insert(isE1Player ? e1 : e2);
                    if (gameState) gameState->IsGameOver = true;
                } else if ((isE1Proj && isE2Ast) || (isE2Proj && isE1Ast)) {
                    entitiesToRemove.insert(e1);
                    entitiesToRemove.insert(e2);
                    if (gameState) gameState->Score++;
                }
            }
        }
    }

    for (int ent: entitiesToRemove) {
        world.RemoveEntity(ent);
    }
}
