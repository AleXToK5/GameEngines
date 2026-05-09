#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include <memory>
#include "../../Ecs/Systems/ISystem.h"
#include "../../Ecs/Filter/Filter.h"
#include "../../Ecs/Filter/FilterBuilder.h"
#include "../../GameEngine/Input/InputAction.h"
#include "TransformComponent.h"
#include "VelocityComponent.h"
#include "GravityComponent.h"
#include "../Gameplay/Player/PlayerComponent.h"

class MovementSystem final : public ISystem {
    ComponentStorage<TransformComponent> &_transforms;
    ComponentStorage<VelocityComponent> &_velocities;
    ComponentStorage<GravityComponent> &_gravities;
    ComponentStorage<PlayerComponent> &_players;

    Filter _moveables;
    Filter _gravityFilter;
    Filter _playersFilter;

    std::shared_ptr<InputAction> _moveLeft;
    std::shared_ptr<InputAction> _moveRight;
    std::shared_ptr<InputAction> _jump;

public:
    MovementSystem(World &world,
                   std::shared_ptr<InputAction> moveLeft,
                   std::shared_ptr<InputAction> moveRight,
                   std::shared_ptr<InputAction> jump)
        : ISystem(world),
          _transforms(world.GetStorage<TransformComponent>()),
          _velocities(world.GetStorage<VelocityComponent>()),
          _gravities(world.GetStorage<GravityComponent>()),
          _players(world.GetStorage<PlayerComponent>()),
          _moveables(FilterBuilder(world).With<TransformComponent>().With<VelocityComponent>().Build()),
          _gravityFilter(FilterBuilder(world).With<VelocityComponent>().With<GravityComponent>().Build()),
          _playersFilter(FilterBuilder(world).With<VelocityComponent>().With<PlayerComponent>().Build()),
          _moveLeft(moveLeft), _moveRight(moveRight), _jump(jump) {
    }

    void OnInit() override {
    }

    void OnUpdate() override {
        for (int e: _playersFilter) {
            auto &vel = _velocities.Get(e);
            auto &player = _players.Get(e);

            if (_moveLeft->Type() == ActionType::Start) {
                vel.X = -player.SpeedX;
            } else if (_moveRight->Type() == ActionType::Start) {
                vel.X = player.SpeedX;
            } else {
                vel.X = 0;
            }

            if (_jump->Type() == ActionType::Start && player.IsGrounded) {
                vel.Y = player.JumpSpeedY;
            }

            player.IsGrounded = false;
        }

        for (int e: _gravityFilter) {
            auto &vel = _velocities.Get(e);
            const auto &grav = _gravities.Get(e);
            vel.Y += grav.Force;
            if (vel.Y > grav.MaxFallSpeed) vel.Y = grav.MaxFallSpeed;
        }

        for (int e: _moveables) {
            auto &t = _transforms.Get(e);
            const auto &v = _velocities.Get(e);
            t.X += v.X;
            t.Y += v.Y;
        }
    }
};

#endif
