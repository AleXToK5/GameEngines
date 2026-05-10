#ifndef PLAYERSTATESYSTEM_H
#define PLAYERSTATESYSTEM_H

#include <cmath>
#include "../../../Ecs/Systems/ISystem.h"
#include "../../../Ecs/Filter/FilterBuilder.h"
#include "../../Physics/VelocityComponent.h"
#include "../../Physics/TransformComponent.h"
#include "../../Graphics/AnimatorComponent.h"
#include "PlayerComponent.h"

class PlayerStateSystem final : public ISystem {
    ComponentStorage<VelocityComponent> &_velocities;
    ComponentStorage<TransformComponent> &_transforms;
    ComponentStorage<AnimatorComponent> &_animators;
    ComponentStorage<PlayerComponent> &_players;
    Filter _filter;
    float _killY;

public:
    PlayerStateSystem(World &world, float windowHeight)
        : ISystem(world),
          _velocities(world.GetStorage<VelocityComponent>()),
          _transforms(world.GetStorage<TransformComponent>()),
          _animators(world.GetStorage<AnimatorComponent>()),
          _players(world.GetStorage<PlayerComponent>()),
          _killY(windowHeight + 200.0f),
          _filter(
              FilterBuilder(world).With<VelocityComponent>().With<TransformComponent>().With<AnimatorComponent>().With<
                  PlayerComponent>().Build()) {
    }

    void OnInit() override {
    }

    void OnUpdate() override {
        for (int e: _filter) {
            auto &vel = _velocities.Get(e);
            auto &t = _transforms.Get(e);
            auto &anim = _animators.Get(e);
            auto &player = _players.Get(e);

            if (t.Y > _killY) {
                t.X = player.SpawnX;
                t.Y = player.SpawnY;
                vel.X = 0.0f;
                vel.Y = 0.0f;
            }

            if (player.ShootAnimTimer > 0) {
                player.ShootAnimTimer--;
            }

            bool isShooting = player.ShootAnimTimer > 0;
            std::string targetAnim = "IdleAnim";
            if (!player.IsGrounded) {
                targetAnim = isShooting ? "ShootJumpAnim" : "JumpAnim";
            } else if (std::abs(vel.X) > 0.1f) {
                targetAnim = isShooting ? "ShootRunAnim" : "RunAnim";
            } else {
                targetAnim = isShooting ? "ShootIdleAnim" : "IdleAnim";
            }

            if (anim.CurrentAnimation != targetAnim) {
                anim.CurrentAnimation = targetAnim;
                anim.CurrentFrame = 0;
                anim.FrameTimer = 0;
            }

            float currentScale = std::abs(t.ScaleX);
            if (vel.X > 0.1f) t.ScaleX = currentScale;
            else if (vel.X < -0.1f) t.ScaleX = -currentScale;
        }
    }
};

#endif
