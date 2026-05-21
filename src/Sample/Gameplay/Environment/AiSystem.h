#ifndef AISYSTEM_H
#define AISYSTEM_H

#include <cmath>
#include "../../../Ecs/Systems/ISystem.h"
#include "../../../Ecs/Filter/FilterBuilder.h"
#include "../../Physics/TransformComponent.h"
#include "../../Physics/VelocityComponent.h"
#include "../../Graphics/AnimatorComponent.h"
#include "GoombaComponent.h"

class AiSystem final : public ISystem {
    ComponentStorage<TransformComponent> &_transforms;
    ComponentStorage<VelocityComponent> &_velocities;
    ComponentStorage<AnimatorComponent> &_animators;
    ComponentStorage<GoombaComponent> &_goombas;
    Filter _filter;

public:
    AiSystem(World &world)
        : ISystem(world),
          _transforms(world.GetStorage<TransformComponent>()),
          _velocities(world.GetStorage<VelocityComponent>()),
          _animators(world.GetStorage<AnimatorComponent>()),
          _goombas(world.GetStorage<GoombaComponent>()),
          _filter(FilterBuilder(world)
              .With<TransformComponent>()
              .With<VelocityComponent>()
              .With<AnimatorComponent>()
              .With<GoombaComponent>()
              .Build()) {
    }

    void OnInit() override {
    }

    void OnUpdate() override {
        const float dt = 1.0f / 60.0f;

        for (int e: _filter) {
            auto &t = _transforms.Get(e);
            auto &v = _velocities.Get(e);
            auto &anim = _animators.Get(e);
            auto &g = _goombas.Get(e);

            if (g.State == GoombaState::Patrol) {
                v.X = g.Direction * g.SpeedX;

                if ((t.X <= g.PatrolLeftX && g.Direction == -1) ||
                    (t.X >= g.PatrolRightX && g.Direction == 1)) {
                    v.X = 0.f;
                    g.State = GoombaState::Idle;
                    g.WaitTimer = 1.0f;
                }
            } else if (g.State == GoombaState::Idle) {
                v.X = 0.f;
                g.WaitTimer -= dt;

                if (g.WaitTimer <= 0.f) {
                    g.Direction *= -1;
                    g.State = GoombaState::Patrol;
                }
            }

            std::string targetAnim = (g.State == GoombaState::Idle) ? "GoombaIdleAnim" : "GoombaWalkAnim";

            if (anim.CurrentAnimation != targetAnim) {
                anim.CurrentAnimation = targetAnim;
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
