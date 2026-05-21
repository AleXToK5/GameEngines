#ifndef AISYSTEM_H
#define AISYSTEM_H

#include <cmath>
#include <unordered_set>
#include "../../../Ecs/Systems/ISystem.h"
#include "../../../Ecs/Filter/FilterBuilder.h"
#include "../../Physics/TransformComponent.h"
#include "../../Physics/VelocityComponent.h"
#include "../../Physics/ColliderComponent.h"
#include "../../Physics/GravityComponent.h"
#include "../../Graphics/AnimatorComponent.h"
#include "../Player/PlayerComponent.h"
#include "../Environment/FinishComponent.h"
#include "GoombaComponent.h"
#include "AStar.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class AiSystem final : public ISystem {
    ComponentStorage<TransformComponent> &_transforms;
    ComponentStorage<VelocityComponent> &_velocities;
    ComponentStorage<AnimatorComponent> &_animators;
    ComponentStorage<GoombaComponent> &_goombas;
    ComponentStorage<ColliderComponent> &_colliders;
    ComponentStorage<GravityComponent> &_gravities;

    Filter _goombaFilter;
    Filter _playerFilter;
    Filter _staticFilter;

    const float VisionRange = 500.f;
    float _windowHeight;

public:
    AiSystem(World &world, float windowHeight)
        : ISystem(world),
          _transforms(world.GetStorage<TransformComponent>()),
          _velocities(world.GetStorage<VelocityComponent>()),
          _animators(world.GetStorage<AnimatorComponent>()),
          _goombas(world.GetStorage<GoombaComponent>()),
          _colliders(world.GetStorage<ColliderComponent>()),
          _gravities(world.GetStorage<GravityComponent>()),
          _windowHeight(windowHeight),
          _goombaFilter(FilterBuilder(world)
              .With<TransformComponent>()
              .With<VelocityComponent>()
              .With<AnimatorComponent>()
              .With<GoombaComponent>()
              .With<GravityComponent>()
              .Build()),
          _playerFilter(FilterBuilder(world)
              .With<TransformComponent>()
              .With<PlayerComponent>()
              .Build()),
          _staticFilter(FilterBuilder(world)
              .With<TransformComponent>()
              .With<ColliderComponent>()
              .Build()) {
    }

    void OnInit() override {
    }

    sf::Vector2i WorldToGrid(const sf::Vector2f &pos) {
        int gridX = static_cast<int>(std::round((pos.x - 32.f) / 64.f));
        int gridY = static_cast<int>(std::round((_windowHeight - pos.y - 32.f) / 64.f));
        return {gridX, gridY};
    }

    sf::Vector2f GridToWorld(const sf::Vector2i &grid) {
        float px = grid.x * 64.f + 32.f;
        float py = _windowHeight - (grid.y * 64.f) - 32.f;
        return {px, py};
    }

    bool HasLineOfSight(const sf::Vector2f &start, const sf::Vector2f &end) {
        float dx = end.x - start.x;
        float dy = end.y - start.y;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist > VisionRange) return false;

        float stepSize = 32.f;
        int steps = static_cast<int>(dist / stepSize);
        if (steps == 0) return true;

        float stepX = dx / steps;
        float stepY = dy / steps;

        for (int i = 1; i < steps; ++i) {
            float px = start.x + stepX * i;
            float py = start.y + stepY * i;

            for (int statEnt: _staticFilter) {
                if (_velocities.Has(statEnt)) continue;
                if (world.GetStorage<FinishComponent>().Has(statEnt)) continue;

                auto &statC = _colliders.Get(statEnt);
                auto &statT = _transforms.Get(statEnt);

                float minX = statT.X - statC.Size.x / 2.f;
                float maxX = statT.X + statC.Size.x / 2.f;
                float minY = statT.Y - statC.Size.y / 2.f;
                float maxY = statT.Y + statC.Size.y / 2.f;

                if (px >= minX && px <= maxX && py >= minY && py <= maxY) {
                    return false;
                }
            }
        }
        return true;
    }

    void OnUpdate() override {
        const float dt = 1.0f / 60.0f;

        int playerEnt = -1;
        for (int p: _playerFilter) {
            playerEnt = p;
            break;
        }

        std::unordered_set<sf::Vector2i, Vec2iHash> solidBlocks;
        for (int statEnt: _staticFilter) {
            if (_velocities.Has(statEnt)) continue;
            if (world.GetStorage<FinishComponent>().Has(statEnt)) continue;

            auto &statT = _transforms.Get(statEnt);
            solidBlocks.insert(WorldToGrid({statT.X, statT.Y}));
        }

        for (int e: _goombaFilter) {
            auto &t = _transforms.Get(e);
            auto &v = _velocities.Get(e);
            auto &anim = _animators.Get(e);
            auto &g = _goombas.Get(e);
            auto &grav = _gravities.Get(e);

            if (playerEnt != -1 && !g.IsJumping) {
                auto &playerT = _transforms.Get(playerEnt);
                sf::Vector2f goombaPos{t.X, t.Y};
                sf::Vector2f playerPos{playerT.X, playerT.Y};

                if (HasLineOfSight(goombaPos, playerPos)) {
                    g.State = GoombaState::Pursue;
                }
            }

            if (g.State == GoombaState::Patrol) {
                if (std::abs(v.X) < 0.01f) {
                    g.Direction *= -1;
                    v.X = g.SpeedX * g.Direction;
                } else {
                    v.X = g.SpeedX * g.Direction;
                    if ((t.X <= g.PatrolLeftX && g.Direction == -1) ||
                        (t.X >= g.PatrolRightX && g.Direction == 1)) {
                        g.Direction *= -1;
                        v.X = g.SpeedX * g.Direction;
                    }
                }
            } else if (g.State == GoombaState::Pursue) {
                if (g.IsJumping) {
                    grav.Force = 0.f;

                    g.JumpTime += dt / 0.4f;
                    if (g.JumpTime >= 1.f) {
                        t.X = g.JumpTarget.x;
                        t.Y = g.JumpTarget.y;
                        g.IsJumping = false;
                        v.Y = 0.f;
                        v.X = 0.f;
                        grav.Force = 0.8f;
                        if (!g.CurrentPath.empty()) {
                            g.CurrentPath.erase(g.CurrentPath.begin());
                        }
                    } else {
                        t.X = g.JumpStart.x + (g.JumpTarget.x - g.JumpStart.x) * g.JumpTime;
                        float arc = std::sin(g.JumpTime * M_PI) * 80.f;
                        t.Y = g.JumpStart.y + (g.JumpTarget.y - g.JumpStart.y) * g.JumpTime - arc;
                        v.X = 0.f;
                        v.Y = 0.f;
                    }
                } else {
                    if (g.CurrentPath.empty() && playerEnt != -1) {
                        auto &playerT = _transforms.Get(playerEnt);
                        sf::Vector2i startGrid = WorldToGrid({t.X, t.Y});
                        sf::Vector2i targetGrid = WorldToGrid({playerT.X, playerT.Y});

                        g.CurrentPath = Pathfinder::FindPath(startGrid, targetGrid, solidBlocks);

                        if (!g.CurrentPath.empty() && g.CurrentPath.front() == startGrid) {
                            g.CurrentPath.erase(g.CurrentPath.begin());
                        }

                        if (g.CurrentPath.empty()) {
                            g.State = GoombaState::Patrol;
                        }
                    }

                    if (!g.CurrentPath.empty()) {
                        sf::Vector2i nextTile = g.CurrentPath.front();
                        sf::Vector2f targetPos = GridToWorld(nextTile);
                        sf::Vector2i currentGrid = WorldToGrid({t.X, t.Y});

                        if (nextTile.y != currentGrid.y) {
                            g.IsJumping = true;
                            g.JumpStart = {t.X, t.Y};
                            g.JumpTarget = targetPos;
                            g.JumpTime = 0.f;
                        } else {
                            g.Direction = (targetPos.x > t.X) ? 1 : -1;
                            v.X = g.SpeedX * g.Direction;

                            if (std::abs(t.X - targetPos.x) <= g.SpeedX) {
                                t.X = targetPos.x;
                                g.CurrentPath.erase(g.CurrentPath.begin());
                            }
                        }
                    } else {
                        v.X = 0.f;
                        if (playerEnt != -1) {
                            g.Direction = (_transforms.Get(playerEnt).X > t.X) ? 1 : -1;
                        }
                    }
                }
            }

            if (anim.CurrentAnimation != "GoombaWalkAnim") {
                anim.CurrentAnimation = "GoombaWalkAnim";
                anim.CurrentFrame = 0;
                anim.FrameTimer = 0;
            }

            float currentScale = std::abs(t.ScaleX);
            if (g.Direction == 1) {
                t.ScaleX = -currentScale;
            } else {
                t.ScaleX = currentScale;
            }
        }
    }
};

#endif // AISYSTEM_H
