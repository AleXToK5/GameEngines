#include "MovementSystem.h"
#include <algorithm>

MovementSystem::MovementSystem(World &world, ConfigReader &config)
    : ISystem(world),
      _config(config),
      _positions(world.GetStorage<PositionComponent>()),
      _movements(world.GetStorage<MovementComponent>()),
      _players(world.GetStorage<PlayerComponent>()),
      _colliders(world.GetStorage<ColliderComponent>()),
      _moveables(FilterBuilder(world)
          .With<PositionComponent>()
          .With<MovementComponent>()
          .Build()) {
}

void MovementSystem::OnUpdate() {
    float screenWidth = (float) _config.GetWindowWidth();
    float screenHeight = (float) _config.GetWindowHeight();

    for (const auto ent: _moveables) {
        auto &position = _positions.Get(ent);
        auto &movement = _movements.Get(ent);

        position.X += movement.Speed * movement.Direction.x;
        position.Y += movement.Speed * movement.Direction.y;

        if (_players.Has(ent)) {
            float radius = 0.f;
            if (_colliders.Has(ent)) {
                radius = _colliders.Get(ent).Radius;
            }

            if (position.X - radius < 0.f) {
                position.X = radius;
            }
            if (position.X + radius > screenWidth) {
                position.X = screenWidth - radius;
            }

            if (position.Y - radius < 0.f) {
                position.Y = radius;
            }
            if (position.Y + radius > screenHeight) {
                position.Y = screenHeight - radius;
            }
        }
    }
}
