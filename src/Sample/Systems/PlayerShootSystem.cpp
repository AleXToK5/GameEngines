#include "PlayerShootSystem.h"

PlayerShootSystem::PlayerShootSystem(World &world, ConfigReader &config)
    : ISystem(world),
      _config(config),
      _players(world.GetStorage<PlayerComponent>()),
      _positions(world.GetStorage<PositionComponent>()),
      _movements(world.GetStorage<MovementComponent>()),
      _renders(world.GetStorage<RenderComponent>()),
      _colliders(world.GetStorage<ColliderComponent>()),
      _projectiles(world.GetStorage<ProjectileComponent>()),
      _playerFilter(FilterBuilder(world)
          .With<PlayerComponent>()
          .With<PositionComponent>()
          .Build()) {
}

void PlayerShootSystem::OnInit() {
}

void PlayerShootSystem::OnUpdate() {
    bool isShooting = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

    for (const auto ent: _playerFilter) {
        auto &player = _players.Get(ent);
        auto &pos = _positions.Get(ent);

        if (player.ShootCooldown > 0) {
            player.ShootCooldown--;
        }

        if (isShooting && player.ShootCooldown <= 0) {
            player.ShootCooldown = player.MaxCooldown;

            int proj = world.CreateEntity();

            _positions.Add(proj, PositionComponent{pos.X, pos.Y - 20.f});

            float projSpeed = _config.GetProjectileSpeed() / 60.0f;
            _movements.Add(proj, MovementComponent{projSpeed, sf::Vector2f(0.f, -1.f)});

            _renders.Add(proj, RenderComponent{ShapeType::Rectangle, 0.f, 0, {10.f, 20.f}, sf::Color::Yellow});
            _colliders.Add(proj, ColliderComponent{ColliderType::AABB, 0.f, {10.f, 20.f}});
            _projectiles.Add(proj, ProjectileComponent{}); // Вешаем тег снаряда
        }
    }
}
